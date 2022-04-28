CC = g++
COMPILE_FLAGS = -g -std=c++17
INCLUDE_FLAGS = -I src/ -I include/
LINK_LIBS = -lncurses -lpthread
# abs_path = $(shell pwd)

# NOTE: dependencies of object files below are not complete
bullet.o: src/bullet.cpp src/bullet.h src/map.h src/player.h src/zombie.h
	$(CC) -c $(COMPILE_FLAGS) $<  $(INCLUDE_FLAGS) -L lib/

map.o: src/map.cpp src/map.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

player.o: src/player.cpp src/player.h src/map.h src/bullet.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

# setting.o: src/setting.cpp src/setting.h 
# 	$(CC) -c $(COMPILE_FLAGS) $< -I src

ui.o: src/ui.cpp src/ui.h src/player.h src/map.h src/zombie.h src/bullet.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

solve_maze.o: src/solve_maze.cpp src/solve_maze.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

zombie.o: src/zombie.cpp src/zombie.h src/bullet.h src/player.h src/map.h src/solve_maze.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

mainloop.o: src/mainloop.cpp src/mainloop.h src/ui.h src/map.h src/bullet.h src/player.h src/zombie.h src/clock.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

main.o: src/main.cpp src/mainloop.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

clock.o: src/clock.cpp src/clock.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

setting.o: src/setting.cpp src/setting.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)

main: bullet.o map.o player.o ui.o zombie.o mainloop.o main.o solve_maze.o clock.o lib/tinyexpr.o setting.o
	g++ $^ -g -o main $(LINK_LIBS)

.PHONY: clean
clean:
	rm bullet.o map.o player.o ui.o zombie.o mainloop.o main.o clock.o solve_maze.o