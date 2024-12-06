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

// Internal method to update enemy position
void Enemy::UpdatePosition() {
    std::lock_guard<std::mutex> lock(position_mutex);

    // Simple random movement (left, right, up, down)
    x += rand() % 3 - 1;  // Random X move (-1, 0, or 1)
    y += rand() % 3 - 1;  // Random Y move (-1, 0, or 1)

    // Keep enemy within grid bounds
    if (x < 0) x = 0;
    if (x >= snake.GetGridWidth()) x = snake.GetGridWidth() - 1;
    if (y < 0) y = 0;
    if (y >= snake.GetGridHeight()) y = snake.GetGridHeight() - 1;
}


// Continuous AI movement method
void Enemy::MoveAI(std::atomic<bool>& game_running) {
    while (game_running.load()) {
        // Move the enemy
        UpdatePosition();
        
        // Sleep to control movement speed
        // Adjust the sleep duration based on the enemy's speed
        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<int>(1000 / (speed * 2))  // Inversely proportional to speed
        ));
    }
}

// Increase the enemy's speed (called when the score increases)
void Enemy::IncreaseSpeed() {
    std::lock_guard<std::mutex> lock(position_mutex);
    speed *= 1.1f;  // Increase the speed incrementally
}

float Enemy::GetSpeed() const {
    return speed;
}

int Enemy::GetX() const { 
    std::lock_guard<std::mutex> lock(position_mutex);
    return x; 
}
int Enemy::GetY() const {
    std::lock_guard<std::mutex> lock(position_mutex);
    return y; 
}
