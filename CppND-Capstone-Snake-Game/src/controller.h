#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Game; //Forward declaration

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake, Game &game) const;
  void StartInputThread(bool &running, Snake &snake, Game &game);
  
 private:
  void ChangeDirection(Snake &snake, Snake::Direction input) const;
};

#endif