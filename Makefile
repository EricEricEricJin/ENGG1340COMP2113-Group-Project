CC = g++
COMPILE_FLAGS = -g -std=c++17
INCLUDE_FLAGS = -I src/ -I include/
LINK_LIBS = -lcurses -lpthread -lstdc++fs

INSTALL_DIR = ${HOME}/.local/bin
RESOURCE_DIR = ${HOME}/.boxhead

.PHONY: create_build all clean install uninstall

all: create_build build/main;

create_build:
	if ! [ -d build ]; \
		then mkdir build; \
	fi;

build/bullet.o: src/bullet.cpp src/bullet.h src/map.h src/player.h src/zombie.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -L lib/ -o $@

build/map.o: src/map.cpp src/map.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/player.o: src/player.cpp src/player.h src/map.h src/bullet.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/ui.o: src/ui.cpp src/ui.h src/player.h src/map.h src/zombie.h src/bullet.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/solve_maze.o: src/solve_maze.cpp src/solve_maze.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/zombie.o: src/zombie.cpp src/zombie.h src/bullet.h src/player.h src/map.h src/solve_maze.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/mainloop.o: src/mainloop.cpp src/mainloop.h src/ui.h src/map.h src/bullet.h src/player.h src/zombie.h src/clock.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@ \
	-D 'BOXHEAD_PATH="$(RESOURCE_DIR)/"'

build/main.o: src/main.cpp src/mainloop.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/clock.o: src/clock.cpp src/clock.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/setting.o: src/setting.cpp src/setting.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

build/rw_saved.o: src/rw_saved.cpp src/rw_saved.h include/json.hpp
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@

lib/tinyexpr.o: lib/tinyexpr.c
	gcc -c $< -o $@ -I include/

build/main: build/bullet.o build/map.o build/player.o build/ui.o build/zombie.o build/mainloop.o build/main.o build/solve_maze.o build/clock.o lib/tinyexpr.o build/setting.o build/rw_saved.o
	g++ $^ -g -o $@ $(LINK_LIBS)

clean:
	rm -r build; \
	rm lib/tinyexpr.o; \

install: build/main local_resource
	if ! [ -d $(INSTALL_DIR) ]; \
		then mkdir $(INSTALL_DIR); \
	fi; \
	cp $< $(INSTALL_DIR)/boxhead; \
	if [ -d $(RESOURCE_DIR) ]; \
		then rm -r $(RESOURCE_DIR); \
	fi; \
	mkdir $(RESOURCE_DIR); \
	mkdir $(RESOURCE_DIR)/saving; \
	cp -r local_resource $(RESOURCE_DIR)/resource; \

uninstall:
	rm $(INSTALL_DIR)/boxhead; \
	rm -r $(RESOURCE_DIR);