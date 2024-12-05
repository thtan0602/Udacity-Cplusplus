#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <set>
#include <utility>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height);
  ~Snake();

  // Rule of 5 functions
  Snake(const Snake& other);  // Copy constructor
  Snake& operator=(const Snake& other);  // Copy assignment operator
  Snake(Snake&& other) noexcept;  // Move constructor
  Snake& operator=(Snake&& other) noexcept;  // Move assignment operator
  
  void Update();
  static Direction GetOppositeDirection(Direction dir);
  void GrowBody();
  bool SnakeCell(int x, int y);
  void UpdateOccupiedCells();  // Add this declaration
  
  // Getter and setter method for current_direction
  Direction& GetCurrentDirection(); 
  void SetCurrentDirection(Direction direction);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  Direction current_direction;

  // Optimized: A set to track the occupied cells
  std::set<std::pair<int, int>> occupied_cells;  // Set of occupied cells
};

#endif