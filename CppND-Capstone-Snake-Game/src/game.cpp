#include <fstream>
#include <iostream>
#include <mutex>
#include "SDL.h"
#include "game.h"

std::mutex snake_mutex;  // Mutex to protect the shared 'snake' object

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

Game::~Game() {
    // Cleanup if necessary (e.g., releasing resources, freeing memory)
}

void Game::Initialize() {
    std::string save_filename = "save_game.dat";
    std::ifstream save_file(save_filename);

    // If the saved game file exists, prompt the user to load it
    if (save_file.good()) {
        std::cout << "Saved game found. Do you want to load it? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            // snake.alive = true;
            LoadGameState(save_filename);  // Load the saved game state
        } else {
            std::cout << "Starting a new game..." << std::endl;
        }
    } else {
        std::cout << "No saved game found. Starting a new game..." << std::endl;
    }
}

  void Game::Run(Controller &controller, Renderer &renderer,
                std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    int frame_count = 0;
    int last_score = 0;
    
    game_running = true; 
    bool running = true;

    StartGameLogicThread();

    controller.StartInputThread(running, snake, *this);

    Enemy enemy(snake);  // Initialize enemy with the grid size

    while (running) {
      frame_start = SDL_GetTicks();

      // Input, Update, Render - the main game loop.
      controller.HandleInput(running, snake, *this);
      Update();
      
      // Move the enemy
      enemy.MoveAI();

      SDL_Point current_head_cell = {static_cast<int>(snake.head_x), static_cast<int>(snake.head_y)};
      
      // Check for collision with enemy (snake dies)
      if (snake.HasCollidedWithEnemy(current_head_cell, enemy)) {
          game_running = false;  // Snake dies, game ends
          std::cout << "Game Over! You hit the enemy!" << std::endl;
          snake.alive = false;
      }

      // If the snake's score has increased, increase the enemy's speed
      if (score > last_score) {
          enemy.IncreaseSpeed();
          last_score = score;  // Update the last score
      }
      
      renderer.Render(snake, food, enemy);

      frame_end = SDL_GetTicks();
      frame_count++;

      // After every second, update the window title with the frame count
      if (frame_end - title_timestamp >= 1000) {
        // Calculate frames per second
        renderer.UpdateWindowTitle(score, frame_count);
        frame_count = 0;  // Reset frame count
        title_timestamp = frame_end;  // Reset timestamp to current time
      }

      // If the time for this frame is too small (i.e., frame_duration is
      // smaller than the target ms_per_frame), delay the loop to
      // achieve the correct frame rate.
      int frame_duration = frame_end - frame_start;  // Calculate how long this frame took
      int delay_time = target_frame_duration - frame_duration;  // How long to delay

      if (delay_time > 0) {
        SDL_Delay(delay_time);  // Delay to maintain the target frame rate
      }
    }

    // Clean up game logic thread when quitting
    game_running = false;

    if (game_logic_thread.joinable()) {
      game_logic_thread.join();  // Ensure the thread finishes properly
    }

    // Call SaveGameState when quitting
    std::string save_file = "save_game.dat";
    SaveGameState(save_file);
  }

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

// Define SaveGameState function
void Game::SaveGameState(const std::string &filePath) {
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for saving game state.\n";
        return;
    }

    // Save game score
    outFile.write(reinterpret_cast<char*>(&score), sizeof(score));

    // Save food position
    outFile.write(reinterpret_cast<char*>(&food.x), sizeof(food.x));
    outFile.write(reinterpret_cast<char*>(&food.y), sizeof(food.y));

    // Save snake direction
    Snake::Direction direction = snake.GetCurrentDirection();
    outFile.write(reinterpret_cast<char*>(&direction), sizeof(direction));

    // Save head position separately (head_x, head_y)
    outFile.write(reinterpret_cast<char*>(&snake.head_x), sizeof(snake.head_x));
    outFile.write(reinterpret_cast<char*>(&snake.head_y), sizeof(snake.head_y));
    
    // Save snake body size
    size_t body_size = snake.body.size();
    outFile.write(reinterpret_cast<char*>(&body_size), sizeof(body_size));

     // Debug: show the food position and the full snake body (head and all body segments)
    std::cout << "Saving food at: (" << food.x << ", " << food.y << ")\n";
    std::cout << "Saving snake head at: (" << snake.head_x << ", " << snake.head_y << ")\n";
    std::cout << "Saving snake body:\n";

    // Save snake body segments
    for (const auto &segment : snake.body) {
        outFile.write(reinterpret_cast<const char*>(&segment), sizeof(segment));
        std::cout << "(" << segment.x << ", " << segment.y << ") ";
    }
    std::cout << "\n";

    std::cout << "Game state saved to " << filePath << "\n";
}

// Define LoadGameState function
void Game::LoadGameState(const std::string &filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for loading game state.\n";
        return;
    }

    // Load game score
    inFile.read(reinterpret_cast<char*>(&score), sizeof(score));

    // Load food position
    inFile.read(reinterpret_cast<char*>(&food.x), sizeof(food.x));
    inFile.read(reinterpret_cast<char*>(&food.y), sizeof(food.y));

    // Load snake direction
    Snake::Direction direction;
    inFile.read(reinterpret_cast<char*>(&direction), sizeof(direction));
    snake.SetCurrentDirection(direction);

    // Load head position separately
    inFile.read(reinterpret_cast<char*>(&snake.head_x), sizeof(snake.head_x));
    inFile.read(reinterpret_cast<char*>(&snake.head_y), sizeof(snake.head_y));

    // Load snake body size
    size_t body_size;
    inFile.read(reinterpret_cast<char*>(&body_size), sizeof(body_size));

    // Validate body size to avoid invalid memory access
    if (body_size > 1000) { // Example maximum size limit
        std::cerr << "Error: Corrupted file. Snake body size too large.\n";
        return;
    }

    // Clear and resize snake body before populating
    snake.body.clear();
    snake.body.resize(body_size);

    // Load snake body segments
    for (size_t i = 0; i < body_size; ++i) {
        SDL_Point segment;
        inFile.read(reinterpret_cast<char*>(&segment), sizeof(segment));
        snake.body[i] = segment;
    }

    // Ensure that the first segment is the head
    snake.body[0] = {static_cast<int>(snake.head_x), static_cast<int>(snake.head_y)};

    // Set snake.size to match the loaded body size
    snake.size = body_size;

    // Debug: show the food position and the full snake body (head and all body segments)
    std::cout << "Loaded food at: (" << food.x << ", " << food.y << ")\n";
    std::cout << "Loaded snake head at: (" << snake.head_x << ", " << snake.head_y << ")\n";
    std::cout << "Loaded snake body:\n";
    for (const auto &segment : snake.body) {
        std::cout << "(" << segment.x << ", " << segment.y << ") ";
    }
    std::cout << "\n";

    std::cout << "Game state loaded from " << filePath << "\n";
}

void Game::Update() {
  if (!snake.alive) return;

  std::lock_guard<std::mutex> lock(snake_mutex);  // Lock mutex to safely access snake state

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

void Game::StartGameLogicThread() {
  game_logic_thread = std::thread([this]() {
    while (game_running.load()) {
      Update();  // Perform game logic (snake movement, food collision)
      SDL_Delay(50);  // Adjust the frequency of updates (e.g., 20 updates per second)
    }
  });
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }