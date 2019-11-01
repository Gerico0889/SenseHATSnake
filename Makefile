CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall


game : game.o fruit.o bomb.o snake.o game_manager.o led_matrix.o joystick.o
	$(CXX) $(CXXFLAGS) -o $@ $^

game.o : game.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

game_manager.o : game_manager.cpp 
	$(CXX) $(CXXFLAGS) -c game_manager.cpp

fruit.o : fruit.cpp
	$(CXX) $(CXXFLAGS) -c fruit.cpp

bomb.o : bomb.cpp
	$(CXX) $(CXXFLAGS) -c bomb.cpp

snake.o : snake.cpp
	$(CXX) $(CXXFLAGS) -c snake.cpp

led_matrix.o : libs/led_matrix.c
	$(CXX) $(CXXFLAGS) -c libs/led_matrix.c

joystick.o : libs/joystick.c
	$(CXX) $(CXXFLAGS) -c libs/joystick.c

clean :
	rm -f *.o game

