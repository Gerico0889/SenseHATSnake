#include "fruit.hpp"

Fruit::Fruit() : fruit_name(Banana), fruit_color(RGB565_YELLOW)
{}

Fruit::Fruit(int name, std::pair<int, int> location)
    : fruit_name(name), fruit_location(location)
{
    if (fruit_name == Banana) {
        fruit_color = RGB565_YELLOW;
        fruit_effect = speed_up;
    }
    else if (fruit_name == Kiwi) {
        fruit_color = RGB565_GREEN;
        fruit_effect = grow;
    }

    else if (fruit_name == Blueberry) {
        fruit_color = RGB565_BLUEBERRY;
        fruit_effect = slow_down;
    }
    else if (fruit_name == Point) {
        fruit_color = RGB565_WHITE;
        fruit_effect = point_up;
    }
}

int
Fruit::get_name() const {
    return fruit_name;
}

uint16_t
Fruit::get_color() const {
    return fruit_color;
}

std::pair<int, int>
Fruit::get_location() const {
    return fruit_location;
}

int
Fruit::get_effect() const {
    return fruit_effect;
}

bool
Fruit::point_spawned() const {
    return spawned_point;
}

void
Fruit::set_point_spawned(const bool x) {
    spawned_point = x;
}
