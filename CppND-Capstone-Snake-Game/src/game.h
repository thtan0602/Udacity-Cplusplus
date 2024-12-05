#ifndef GAME_H
#define GAME_H

#include <random>
#include <atomic>
#include <thread>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void StartGameLogicThread();

  int GetScore() const;
  int GetSize() const;
  
  void SaveGameState(const std::string &filename);// Code to serialize and save the game state to a file
  void LoadGameState(const std::string &filename);// Code to deserialize and load the game state from a file
  void Initialize();

 private:
  Snake snake;
  SDL_Point food;

  std::atomic<bool> game_running;
  std::thread game_logic_thread;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif