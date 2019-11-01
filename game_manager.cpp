#include "game_manager.hpp"

GameManager::GameManager()
  :  color(RGB565_CYAN), lost(false), points(5), snake{}, bomb{}
{
  if (open_led_matrix() == -1)
    throw std::invalid_argument("Cannot open LED Matrix");

  open_joystick_device();
}

void
GameManager::lose() {
  lost = true;
}

void
GameManager::reset() {
  lost = false;
  points = 5;
  snake = Snake{};
}

void
GameManager::display_snake() {
  for (auto &x : snake.get_body()) {
    set_led(x.first, x.second, color);
  }
}

void
GameManager::display_fruit() {
  for (auto &x : fruit_vec) {
    auto location = x.get_location();
    set_led(location.first, location.second, x.get_color());
  }
}

void
GameManager::display_bomb() {
  int tick = 800;
  for (int i = 0; i < 10; ++i) {
    if (i == 3)
      tick = 400;
    else if (i == 6)
      tick = 200;
    set_led(bomb.get_location().first, bomb.get_location().second, RGB565_RED);
    std::this_thread::sleep_for(std::chrono::milliseconds(tick));
    set_led(bomb.get_location().first, bomb.get_location().second, RGB565_OFF);
    std::this_thread::sleep_for(std::chrono::milliseconds(tick));
  }
}

void
GameManager::increase_points() {
  ++points;
}

void
GameManager::increase_points(const int x) {
  points += x;
}

void
GameManager::decrease_points() {
  --points;
}

void
GameManager::decrease_points(const int x) {
  points -= x;
}

void
GameManager::move() {
  int pos = snake.get_direction();

  if (pos == DIRECTION_NORTH) {
    snake.get_body().pop_back();
    snake.get_body().push_front(
      std::make_pair(snake.get_body().front().first - 1,
                     snake.get_body().front().second));
  }
  else if (pos == DIRECTION_SOUTH) {
    snake.get_body().pop_back();
    snake.get_body().push_front(
      std::make_pair(snake.get_body().front().first + 1,
                     snake.get_body().front().second));
  }
  else if (pos == DIRECTION_WEST) {
    snake.get_body().pop_back();
    snake.get_body().push_front(
      std::make_pair(snake.get_body().front().first,
                     snake.get_body().front().second - 1));
  }
  else if (pos == DIRECTION_EAST) {
    snake.get_body().pop_back();
    snake.get_body().push_front(
      std::make_pair(snake.get_body().front().first,
                     snake.get_body().front().second + 1));
  }

  valid_head_position();
  display_snake();
  usleep((1000 * snake.get_speed()));

  set_led(snake.get_body().back().first, snake.get_body().back().second,
          RGB565_OFF);
}

void
GameManager::handle_input() {
  int prev;

  while (1) {
    prev = snake.get_direction();

    ev = read_joystick_input();
    if (ev.type == JOYSTICK_PRESS) {
      if (ev.direction == DIRECTION_NORTH && prev != DIRECTION_SOUTH) {
        snake.set_direction(ev.direction);
      } else if (ev.direction == DIRECTION_SOUTH && prev != DIRECTION_NORTH) {
        snake.set_direction(ev.direction);
      } else if (ev.direction == DIRECTION_WEST && prev != DIRECTION_EAST) {
        snake.set_direction(ev.direction);
      } else if (ev.direction == DIRECTION_EAST && prev != DIRECTION_WEST) {
        snake.set_direction(ev.direction);
      } else if (ev.direction == DIRECTION_DOWN) {
        //stop = true;
      }
    }
    else if (ev.type == JOYSTICK_HOLD) {
      if (ev.direction == DIRECTION_DOWN) {
		stop = true;
      }
    }

    if (has_lost() || has_stop())
      break;
  }
}

bool
GameManager::valid_position() {
  if ((0 <= snake.get_body().front().first && snake.get_body().front().first < 8) &&
      (0 <= snake.get_body().front().second && snake.get_body().front().second < 8))
    return true;
  else
    return false;
}

void
GameManager::valid_head_position() {
  if (snake.get_body().front().first < 0)
    snake.get_body().front().first = 7;
  else if (snake.get_body().front().first > 7)
    snake.get_body().front().first = 0;
  else if (snake.get_body().front().second < 0)
    snake.get_body().front().second = 7;
  else if (snake.get_body().front().second > 7)
    snake.get_body().front().second = 0;

  auto first = snake.get_body().begin();
  ++first;
  for (; first != snake.get_body().end(); ++first) {
    if (*first == snake.get_body().front())
      lose();
  }

}

std::pair<int, int>
GameManager::spawn_location() {
  std::vector<std::pair<int, int>> valid_pos;

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      valid_pos.emplace_back(i, j);

  // Remove bomb from vector
  if (bomb.is_spawned()) {
    auto expl_range = bomb.explode();
    for (const auto &loc : expl_range)
      valid_pos.erase(std::remove(valid_pos.begin(), valid_pos.end(), loc));
  }

  // Remove fruits
  if (fruit_vec.size() != 0)
    for (auto first = std::begin(fruit_vec); first != std::end(fruit_vec); ++first)
      valid_pos.erase(std::remove(valid_pos.begin(), valid_pos.end(), first->get_location()));

  // Remove point
  if (point.point_spawned())
    valid_pos.erase(std::remove(valid_pos.begin(), valid_pos.end(), point.get_location()));

  // Remove snake
  for (const auto &snake_cell : snake.get_body())
    valid_pos.erase(std::remove(valid_pos.begin(), valid_pos.end(), snake_cell));

 
  int index = rand() % valid_pos.size();
  auto selected = valid_pos.at(index);

  return selected;
}

void
GameManager::spawn_point() {
  if (point.point_spawned())
    return;

  std::pair<int, int> location = spawn_location();
  Fruit point_tmp(Point, location);

  point = point_tmp;
  point.set_point_spawned(true);
  set_led(point.get_location().first, point.get_location().second, point.get_color());
}

void
GameManager::spawn_fruit() {
  while (1) {
    int random = rand() % 100;
    std::pair<int, int> location = spawn_location();
    Fruit curr_fruit;

    if (0 <= random && random <= 10)
      curr_fruit = Fruit(Banana, location);
    else if (11 <= random && random <= 80)
      curr_fruit = Fruit(Kiwi, location);
    else
      curr_fruit = Fruit(Blueberry, location);

    fruit_vec.push_back(curr_fruit);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (has_lost() || has_stop())
      break;
  }
}

void
GameManager::manage_fruit() {
  if (point.get_location() == snake.get_body().front()) {
    increase_points();
    point.set_point_spawned(false);
    set_led(point.get_location().first, point.get_location().second, RGB565_OFF);
  }

  if (fruit_vec.size() == 0)
    return;

  int index = -1;
  for (auto &x : fruit_vec) {
    ++index;
    if (x.get_location() == snake.get_body().front()) {
      if (x.get_effect() == speed_up)
        snake.set_speed(snake.get_speed() + 70);
      else if (x.get_effect() == slow_down)
        snake.set_speed(snake.get_speed() - 70);
      else if (x.get_effect() == grow)
        snake.increase_size();

      fruit_vec.erase(std::begin(fruit_vec) + (index));
    }
  }

}

void
GameManager::spawn_bomb() {
  std::vector<std::pair<int,int>> borders;
  for (int i = 0; i < 8; ++i) {
    borders.emplace_back(0,i);
    borders.emplace_back(7,i);
    borders.emplace_back(i,0);
    borders.emplace_back(i,7);
  }

  while (1) {
    if (bomb.is_spawned())
      std::this_thread::sleep_for(std::chrono::seconds(5));

    // If the spawned location is on the border, generate a new one
    auto loc = spawn_location();
    if (std::find(borders.begin(), borders.end(), loc) != borders.end())
      continue;

    bomb.set_location(loc);
    bomb.set_spawned(true);
    display_bomb();
    set_exploded(true);

    if (has_lost() || has_stop())
      break;
  }
}

void
GameManager::manage_bomb() {
  auto explosion_range = bomb.explode();

  auto async_explosion = [&explosion_range](Fruit &point) {
    // Show explosion;
    for (const auto &led : explosion_range)
      set_led(led.first, led.second, RGB565_GRAY);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    for (const auto &led : explosion_range)
      set_led(led.first, led.second, RGB565_OFF);

    set_led(point.get_location().first, point.get_location().second, point.get_color());
  };

  auto async_snake_blink = [](std::list<std::pair<int,int>> snek) {
    for (int i = 0; i < 4; ++i) {
      for (const auto &snek_led : snek)
        set_led(snek_led.first, snek_led.second, RGB565_RED);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      for (const auto &snek_led : snek)
        set_led(snek_led.first, snek_led.second, RGB565_OFF);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  };

  // Async call for showing explosion
  std::async(std::launch::async, async_explosion, std::ref(point));

  auto snake_first = std::begin(snake.get_body());
  auto snake_last = std::end(snake.get_body());
  auto expl_first = std::begin(explosion_range);
  auto expl_last = std::end(explosion_range);

  // See if the snake was not in the explosion range
  if (std::find_first_of(snake_first, snake_last, expl_first, expl_last)
      == snake_last)
  {
    set_exploded(false);
    return;
  }

  if (snake.get_body().size() <= 3) {
    decrease_points(5);
    lose();
  } // Turn off LEDs and remove element from the snakes list
  else {
    int index = 0;
    for (auto first = snake.get_body().rbegin(); index <= 2; ++first, ++index)
      set_led((*first).first, (*first).second, RGB565_OFF);

    snake.get_body().pop_back();
    snake.get_body().pop_back();
    std::async(std::launch::async, async_snake_blink, snake.get_body());

    decrease_points(5);
    set_exploded(false);
  }
}

bool
GameManager::has_lost() const {
  return lost;
}

int
GameManager::get_points() const {
  return points;
}
