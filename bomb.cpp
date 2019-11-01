#include "bomb.hpp"
#include <iostream>

Bomb::Bomb(std::pair<int,int> loc) : location(loc), spawned(false)
{
}

std::pair<int,int>
Bomb::get_location() const {
  return location;
}

bool
Bomb::is_spawned() const {
  return spawned;
}

void
Bomb::set_location(std::pair<int,int> loc){
  location = loc;
}

void
Bomb::set_spawned(const bool value) {
  spawned = value;
}

std::vector<std::pair<int,int>>
Bomb::explode() {
  std::vector<std::pair<int,int>> explosion_range;
  auto loc = get_location();

  // Create a vector containing range of explosion
  for (int x = -1; x < 2; ++x)
    for (int y = -1; y < 2; ++y)
      explosion_range.emplace_back(loc.first + x, loc.second + y);

  return explosion_range;
}
