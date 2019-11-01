#include <stdexcept>

#include "snake.hpp"
#include "libs/joystick.h"

Snake::Snake() : direction(DIRECTION_NORTH), head{4,4}, tail{5,4}, speed(400)
{
  body.push_back(head);
  body.push_back(tail);
}

// Snake::Snake(const int s, const int x, const int y)
//   : size(s), row(x), col(y)
// {
//   if (size <= 0)
//     throw std::invalid_argument("Size can't be less or equal to zero");
//   if (row < 0 || row >= 64)
//     throw std::invalid_argument("Row must be between 0 and 63");
//   if (col < 0 || col >= 64)
//     throw std::invalid_argument("Col must be between 0 and 63");

// }

void
Snake::increase_size() {
  std::pair<int, int> new_elem;

  if (direction == DIRECTION_NORTH)
    new_elem = std::make_pair(tail.first + 1, tail.second);
  else if (direction == DIRECTION_SOUTH)
    new_elem = std::make_pair(tail.first - 1, tail.second);
  else if (direction == DIRECTION_WEST)
    new_elem = std::make_pair(tail.first, tail.second - 1);
  else if (direction == DIRECTION_EAST)
    new_elem = std::make_pair(tail.first, tail.second + 1);

  body.push_back(new_elem);
}

void
Snake::decrease_size() {
  body.pop_back();
}

int
Snake::get_size() const {
  return body.size();
}

int
Snake::get_direction() const {
  return direction;
}

int
Snake::get_speed() const {
  return speed;
}

std::list<std::pair<int, int>>&
Snake::get_body() {
  return body;
}

void
Snake::set_direction(const int new_dir) {
  direction = new_dir;
}

void
Snake::set_speed(const int new_speed) {
  speed = new_speed;
}
