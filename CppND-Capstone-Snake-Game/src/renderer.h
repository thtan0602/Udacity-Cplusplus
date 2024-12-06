#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "enemy.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(const Snake& snake, const SDL_Point& food, const Enemy& enemy);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  // Utility functions to render specific elements
  void RenderFood(const SDL_Point& food, SDL_Rect& block);
  void RenderSnake(const Snake& snake, SDL_Rect& block);
  void RenderEnemy(const Enemy& enemy, SDL_Rect& block); // Rendering the enemy
  void ClearScreen();
};

#endif