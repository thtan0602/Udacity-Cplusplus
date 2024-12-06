#include <cstdlib>  // For rand()
#include <ctime>
#include "enemy.h"
#include "snake.h"

// Constructor to initialize the enemy position
Enemy::Enemy(Snake& snake)
    : snake(snake), speed(0.5f), alive(true) {
    // Initialize enemy's position randomly within the grid
    srand(time(0));  // Initialize random number generator with current time
    x = rand() % snake.GetGridWidth();  // Random x position based on Snake's grid width
    y = rand() % snake.GetGridHeight(); // Random y position based on Snake's grid height
}

Enemy::~Enemy() {
    // Empty destructor if no special cleanup is needed
}

// AI movement for the enemy (for now it's random movement)
void Enemy::MoveAI() {
    // Simple random movement (left, right, up, down)
    x += rand() % 3 - 1;  // Random X move (-1, 0, or 1)
    y += rand() % 3 - 1;  // Random Y move (-1, 0, or 1)

    // Keep enemy within grid bounds
    if (x < 0) x = 0;
    if (x >= snake.GetGridWidth()) x = snake.GetGridWidth() - 1;
    if (y < 0) y = 0;
    if (y >= snake.GetGridHeight()) y = snake.GetGridHeight() - 1;
}

// Increase the enemy's speed (called when the score increases)
void Enemy::IncreaseSpeed() {
    speed += 0.001f;  // Increase the speed incrementally
}

float Enemy::GetSpeed() const {
    return speed;
}

int Enemy::GetX() const { return x; }
int Enemy::GetY() const { return y; }
