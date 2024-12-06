# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

##For project submission
1. Project Overview
A brief introduction to what the project is about, its purpose, and what the game does.

# SnakeGame
This is a simple implementation of the classic Snake game built using SDL2 and C++17. The game includes several new features such as multi-threading and custom controls.

2. Building and Running Instructions
Instructions on how to build and run the project, with cross-platform installation instructions if additional libraries are required (like SDL2 or pthread).

## Building and Running
### Prerequisites:
- C++17 compatible compiler
- SDL2 library (see instructions below for installation)
- pthread library (for multithreading functionality)

### For Linux:
1. Install SDL2:
   ```bash
   sudo apt-get install libsdl2-dev
   ```

3. Clone the repository and navigate to the project folder:
   ```bash
   git clone git@github.com:thtan0602/Udacity-Cplusplus.git
   Udacity C++/CppND-Capstone-Snake-Game
   ```

4. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

5. Run the game:
   ```bash
   ./SnakeGame
   ```

### For Windows and macOS:
(Provide platform-specific instructions for SDL2 installation and building if necessary) (I use ubuntu 22.04 btw)

3. New Features Added
## New Features

### Multithreading:
- Added multi-threading support using `pthread` to handle game updates and rendering concurrently, ensuring smoother gameplay.

### Improved User Experience:
- User would get to choose whether they want to use the saved data to play the game so that the user could pause the game while they have something else that is more urgent to do and could continue playing the game from where they left.

### Locks
- The controller and game can lock the operation during changing of direction and updating of game state  

### Enemy
- Include an enemy in enemy.hpp and enemy.cpp to increase the difficulty of the game. If the snake scores more and moves faster, the enemy will catch up as well.

4. Rubric Points Addressed
Indicate the rubric points you have addressed in your project. This helps the reviewer understand how your project aligns with the grading rubric. Also, refer to where in the code each point is addressed (e.g., filenames and line numbers).

## Rubric Points Addressed
- Multithreading (see `controller.cpp`, lines 18-21, `game.cpp`, line 51-53, lines 88-90, lines 237-244)
- Improved user experience (see `game.cpp`, lines 21-39)
- Locks (see `controller.cpp`, line 9 and 12, `game.cpp`, line 7 and 220)
- Enemy (see `enemy.h` and `enemy.cpp`)

- README done
- Compiling and Testing done
- Loops, Functions, I/O
  - The project demonstrates an understanding of C++ functions and control structures.
  - The project reads data from a file and process the data, or the program writes data to a file.
  - The project accepts user input and processes the input.
- Object Oriented Programming 
  - One or more classes are added to the project with appropriate access specifiers for class members.
  - Class constructors utilize member initialization lists.
  - Classes abstract implementation details from their interfaces.
- Memory Management
  - The project makes use of references in function declarations.
  - The project uses destructors appropriately.
  - The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
  - The project follows the Rule of 5. (for snake. h and snake.cpp)
  - The project uses move semantics to move data instead of copying it, where possible.
  - The project uses smart pointers instead of raw pointers.
- Concurrency
  - The project uses multithreading.
  - A mutex or lock is used in the project.

5. Expected Behavior or Output
Provide a section that describes the expected output or behavior of the game, ensuring that it meets the requirements specified in the rubric.

## Expected Behavior
- The game should prompt the user to choose whether or not to use the saved data before starting the game
- The game should start with a snake and food in the game window.
- The player can control the snake using the arrow keys and quit using escape key.
- The snake should grow in length when it eats food, and the game should end when the snake collides with itself or the wall.
- New textures should be visible for the snake and food, and the game should run smoothly with no frame rate drops.
- The enemy will move faster when the snake scores more and moves faster
- The snake will die if it hits the enemy

6. Conclusion
A brief conclusion and any additional comments you think are important for the reviewer.
## Conclusion

This project implements a classic Snake game with several new features. The use of multithreading improves performance, the addition of custom controls and locks and improved user experience. I hope you enjoy playing the game and reviewing the code!

