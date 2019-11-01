#include <cstdlib>
#include <ctime>

#include "game_manager.hpp"

int main() {
  GameManager manager;

  srand(time(NULL));

  std::thread t_move(&GameManager::handle_input, &manager);
  std::thread t_fruit(&GameManager::spawn_fruit, &manager);
  std::thread t_bomb(&GameManager::spawn_bomb, &manager);

  while (!manager.has_stop() && !manager.has_lost()) {
    manager.spawn_point();
    manager.display_fruit();
    if (manager.has_exploded()) {
      manager.manage_bomb();
    }
    manager.move();
    manager.manage_fruit();
  }

  clear_leds();
  
  if (manager.has_stop()) { // Player has stopped the game
    std::string command {"python display_message.py GAME_STOP "};
    command.append(std::to_string(manager.get_points()));
    system(command.c_str());
  }
  else {
    std::string command {"python display_message.py GAME_OVER "};
    command.append(std::to_string(manager.get_points()));
    system(command.c_str());
  }

  close_led_matrix();
  close_joystick_device();
}
