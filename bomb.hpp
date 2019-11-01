#ifndef __BOMB_H_
#define __BOMB_H_

#include <utility>
#include <list>
#include <vector>
#include <algorithm>

#include "libs/led_matrix.h"

class Bomb {
  public:
    Bomb() = default;
    Bomb(std::pair<int,int> location);

    std::pair<int,int> get_location() const;
    bool is_spawned() const;

    void set_location(const std::pair<int,int> loc);
    void set_spawned(const bool value);

    std::vector<std::pair<int,int>> explode();
  private:
    std::pair<int,int> location;
    bool spawned;
};


#endif // __BOMB_H_
