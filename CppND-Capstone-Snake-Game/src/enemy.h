#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <mutex>
#include <atomic>
#include <thread>

class Snake;  // Forward declaration of Snake class

class Enemy {
public:
    // Constructor to initialize the enemy
    Enemy(Snake& snake);
    ~Enemy();

    // Move the enemy AI (random movement for this example)
    void MoveAI(std::atomic<bool>& game_running);

    // Getters for enemy position
    int GetX() const;
    int GetY() const;

    // Speed and movement direction
    void IncreaseSpeed();  // Increase the enemy's speed
    float GetSpeed() const; // Get the current speed

private:
    bool alive; // Whether the enemy is alive
    int x, y;  // Position of the enemy on the grid
    Snake& snake;
    float speed;  // Speed of the enemy
    mutable std::mutex position_mutex;  // Mutex to protect position updates

    // Internal method for random movement
    void UpdatePosition();
};

#endif // ENEMY_H
