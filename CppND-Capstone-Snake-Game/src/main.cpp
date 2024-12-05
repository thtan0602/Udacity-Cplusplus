#include <iostream>
#include <memory>  // Include for smart pointers

#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    auto game = std::make_unique<Game>(kGridWidth, kGridHeight);

    game->Initialize();  // Check for saved game and initialize accordingly

    // Create smart pointers for Renderer, Controller, and Game
    auto renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    auto controller = std::make_unique<Controller>();

    // Run the game
    game->Run(*controller, *renderer, kMsPerFrame);

    // Print game result
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game->GetScore() << "\n";
    std::cout << "Size: " << game->GetSize() << "\n";

    return 0;
}
