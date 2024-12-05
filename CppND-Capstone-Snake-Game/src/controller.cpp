#include <iostream>
#include <mutex>

#include "SDL.h"
#include "snake.h"
#include "game.h"
#include "controller.h"

std::mutex controller_mutex;  // Mutex to protect the shared 'snake' object

void Controller::ChangeDirection(Snake &snake, Snake::Direction input) const {
  std::lock_guard<std::mutex> lock(controller_mutex);  // Lock mutex before modifying snake

  if (snake.direction != Snake::GetOppositeDirection(input) || snake.size == 1) snake.direction = input;
  return;
}

void Controller::StartInputThread(bool &running, Snake &snake, Game &game) {
  std::thread input_thread(&Controller::HandleInput, this, std::ref(running), std::ref(snake), std::ref(game));
  input_thread.detach();  // Detach the thread to let it run independently
}

void Controller::HandleInput(bool &running, Snake &snake, Game &game) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight);
          break;

        case SDLK_ESCAPE:  // Add an escape key to stop the game
          running = false;  // Set running to false to stop the game
          break;

        // case SDLK_s:  // Save game state when 'S' is pressed
        //   {
        //     std::string save_filename = "save_game.dat";
        //     game.SaveGameState(save_filename);  // Call Save function
        //     std::cout << "Game state saved to " << save_filename << std::endl;
        //   }
        //   break;

        // case SDLK_l:  // Load game state when 'L' is pressed
        //   {
        //     std::string load_filename = "save_game.dat";
        //     game.LoadGameState(load_filename);  // Call Load function
        //     std::cout << "Game state loaded from " << load_filename << std::endl;
        //   }
        //   break;
        default:
          std::cout << "Unrecognized key pressed!" << std::endl;
          break;
      }
    }
  }
}