#include "blockade.h"

#include <algorithm>
#include <cmath>
#include <iostream>

void Blockade::Update(int x, int y) {
  point.x = static_cast<int>(head_x);
  point.y = static_cast<int>(head_y);

  if (abs(head_x - x) > abs(head_y - y)) {
    if (x < static_cast<int>(head_x)) {
      direction = Direction::kLeft;
    } else {
      direction = Direction::kRight;
    }
  } else {
    if (y < static_cast<int>(head_y)) {
      direction = Direction::kUp;
    } else {
      direction = Direction::kDown;
    }
  }

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

  // Wrap  to the beginning if going off of the screen.
  head_x = fmod(head_x + _grid_width, _grid_width);
  head_y = fmod(head_y + _grid_height, _grid_height);
}
