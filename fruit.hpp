#ifndef __FRUIT_H_
#define __FRUIT_H_

#include <utility>
#include <list>
#include <string>

#include "libs/led_matrix.h"

enum FRUITS {Banana, Blueberry, Kiwi, Point};
enum EFFECTS {speed_up, slow_down, grow, point_up};

class Fruit {
        public:
                Fruit();
                Fruit(int name, std::pair<int, int> location);
        
                int get_name() const;
                uint16_t get_color() const;
                std::pair<int, int> get_location() const;
                int get_effect() const;

                bool point_spawned() const;
                void set_point_spawned(const bool x);

        private:

                int fruit_name;
                int fruit_effect;
                uint16_t fruit_color;
                std::pair<int, int> fruit_location;
                bool spawned_point{false};
};

#endif // __FRUIT_H_
