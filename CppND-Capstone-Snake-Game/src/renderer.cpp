#include <iostream>
#include <string>

#include "renderer.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (sdl_window == nullptr) {
    SDL_Quit();
    throw std::runtime_error("Window could not be created. SDL_Error: " + std::string(SDL_GetError()));
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl_renderer == nullptr) {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    throw std::runtime_error("Renderer could not be created. SDL_Error: " + std::string(SDL_GetError()));
  }
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::ClearScreen() {
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
}

void Renderer::RenderFood(const SDL_Point& food, SDL_Rect& block) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);  // Yellow for food
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderSnake(const Snake& snake, SDL_Rect& block) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);  // Blue for snake
    for (const SDL_Point& point : snake.body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake.head_x) * block.w;
    block.y = static_cast<int>(snake.head_y) * block.h;
    if (snake.alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);  // Snake head color
    } else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red if dead
    }
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderEnemy(const Enemy& enemy, SDL_Rect& block) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red for enemy
    block.x = enemy.GetX() * block.w;
    block.y = enemy.GetY() * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::Render(const Snake& snake, const SDL_Point& food, const Enemy& enemy) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Clear screen
    ClearScreen();

    // Render food, snake, and enemy
    RenderFood(food, block);
    RenderSnake(snake, block);
    RenderEnemy(enemy, block);  // Render enemy here

    // Update the screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
