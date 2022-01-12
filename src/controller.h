#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

 private:
  static void ChangeDirection(Snake &snake, Snake::Direction input,
                              Snake::Direction opposite);
};

#endif
