#ifndef __GAME_MANAGER_H_
#define __GAME_MANAGER_H_

#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <future>

#include "snake.hpp"
#include "fruit.hpp"
#include "bomb.hpp"
#include "libs/led_matrix.h"
#include "libs/joystick.h"

class GameManager {
        public:
                GameManager();

                void lose();
                void reset();
                void display_snake();
                void display_fruit();
                void display_bomb();
                void increase_points();
                void increase_points(const int x);
                void decrease_points();
                void decrease_points(const int x);
                void move();
                void handle_input();
                bool valid_position();
                void valid_head_position();
                std::pair<int, int> spawn_location();
                void spawn_point();
                void spawn_fruit();
                void manage_fruit();
                void spawn_bomb();
                void manage_bomb();

                bool has_lost() const;
                int get_points() const;
                bool has_stop() const {
                        return stop;
                }
                bool has_exploded() const {
                        return bomb_exploded;
                }
                void set_exploded(const bool x) {
                        bomb_exploded = x;
                }

        private:
                int color;
                bool lost;
                bool stop;
                bool bomb_exploded {false};
                int points;
                Snake snake;
                Fruit point;
                std::vector<Fruit> fruit_vec;
                Bomb bomb;
                js_event ev;
};

#endif // __GAME_MANAGER_H_
