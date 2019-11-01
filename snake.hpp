#ifndef __SNAKE_H_
#define __SNAKE_H_

#include <utility>
#include <list>
#include <string>

class Snake {
        public:
                Snake();
                //    Snake(const int size, const int row, const int col);
                void increase_size();
                void decrease_size();
                void set_position(const int row, const int col);

                int get_size() const;
                int get_direction() const;
                int get_speed() const;

                std::list<std::pair<int, int>>& get_body();

                void set_direction(const int new_dir);
                void set_speed(const int new_speed);
        private:
                int direction;
                std::list<std::pair<int,int>> body;
                std::pair<int, int> head;
                std::pair<int, int> tail;
                int speed;
};

#endif // __SNAKE_H_
