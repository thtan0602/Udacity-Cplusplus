#include <cmath>
#include <iostream>

#include "snake.h"

Snake::Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

Snake::~Snake() {
    // Cleanup if necessary
}

Snake::Snake(const Snake& other)
    : grid_width(other.grid_width),
      grid_height(other.grid_height),
      head_x(other.head_x),
      head_y(other.head_y),
      direction(other.direction),
      speed(other.speed),
      size(other.size),
      alive(other.alive),
      body(other.body),  // Deep copy of the body vector
      occupied_cells(other.occupied_cells)  // Deep copy of the set
{
}

Snake& Snake::operator=(const Snake& other) {
  if (this != &other) {  // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    head_x = other.head_x;
    head_y = other.head_y;
    direction = other.direction;
    speed = other.speed;
    size = other.size;
    alive = other.alive;
    body = other.body;  // Deep copy of the body vector
    occupied_cells = other.occupied_cells;  // Deep copy of the set
  }
  return *this;
}

Snake::Snake(Snake&& other) noexcept
    : grid_width(other.grid_width),
      grid_height(other.grid_height),
      head_x(other.head_x),
      head_y(other.head_y),
      direction(other.direction),
      speed(other.speed),
      size(other.size),
      alive(other.alive),
      body(std::move(other.body)),  // Move the body vector
      occupied_cells(std::move(other.occupied_cells))  // Move the set
{
  other.head_x = 0;
  other.head_y = 0;
  other.size = 0;
  other.alive = false;
}

Snake& Snake::operator=(Snake&& other) noexcept {
  if (this != &other) {  // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    head_x = other.head_x;
    head_y = other.head_y;
    direction = other.direction;
    speed = other.speed;
    size = other.size;
    alive = other.alive;
    body = std::move(other.body);  // Move the body vector
    occupied_cells = std::move(other.occupied_cells);  // Move the set

    other.head_x = 0;
    other.head_y = 0;
    other.size = 0;
    other.alive = false;
  }
  return *this;
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(std::move(prev_head_cell));

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // // Check if the snake has collided with its own body
  if (HasCollidedWithBody(current_head_cell)) {
      alive = false;
  }
}

// Check if the snake collides with its own body
bool Snake::HasCollidedWithBody(SDL_Point &current_head_cell) const {
    for (auto const &item : body) {
        if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

// Check if the snake collides with the enemy
bool Snake::HasCollidedWithEnemy(SDL_Point &current_head_cell, const Enemy &enemy) const {
    return current_head_cell.x == enemy.GetX() && current_head_cell.y == enemy.GetY();
}

Snake::Direction Snake::GetOppositeDirection(Snake::Direction dir) {
  switch (dir) {
    case Snake::Direction::kUp: return Snake::Direction::kDown;
    case Snake::Direction::kDown: return Snake::Direction::kUp;
    case Snake::Direction::kLeft: return Snake::Direction::kRight;
    case Snake::Direction::kRight: return Snake::Direction::kLeft;
  }
}

void Snake::GrowBody() { growing = true; }

// Optimized SnakeCell using a set
bool Snake::SnakeCell(int x, int y) {
    // Check if the (x, y) position is in the set of occupied cells
    return occupied_cells.count({x, y}) > 0;
}

// Function to update the occupied_cells set when the snake moves or grows
void Snake::UpdateOccupiedCells() {
    occupied_cells.clear();  // Clear previous occupied cells
    occupied_cells.insert({head_x, head_y});  // Insert the head position
    for (const auto &item : body) {
        occupied_cells.insert({item.x, item.y});  // Insert each body part position
    }
}

// Getter method for current_direction
Snake::Direction& Snake::GetCurrentDirection(){
    return current_direction;
}

// Setter method for current_direction
void Snake::SetCurrentDirection(Snake::Direction direction) {
    current_direction = direction;
}

