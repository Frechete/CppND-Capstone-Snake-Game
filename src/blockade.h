#ifndef BLOCKADE_H
#define BLOCKADE_H

#include "SDL.h"

class Blockade {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Blockade(int grid_width, int grid_height)
      : _grid_width(grid_width),
        _grid_height(grid_height),
        head_x(grid_width),
        head_y(grid_height) {}

  void Update(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.01f};

  float head_x;
  float head_y;
  SDL_Point point;

 private:
  int _grid_width;
  int _grid_height;
};

#endif
