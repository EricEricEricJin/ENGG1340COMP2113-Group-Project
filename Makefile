CC = g++
COMPILE_FLAGS = -std=c++17
abs_path = $(shell pwd)

.PHONY: install_ncurses uninstall_ncurses clean
install_ncurses:
	cd $(abs_path)/ncurses-6.3; \
	./configure --prefix=$(abs_path) --enable-widec; \
	make uninstall; \
	make clean; \
	make -j8; \
	make install.includes; \
	make install.data; \
	make install.libs; \

# NOTE: dependencies of object files below are not complete
bullet.o: src/bullet.cpp src/bullet.h
	$(CC) -c $(COMPILE_FLAGS) \
	-lpthread \
	$< -I src

map.o: src/map.cpp src/map.h
	$(CC) -c $(COMPILE_FLAGS) $< -I src

player.o: src/player.cpp src/player.h
	$(CC) -c $(COMPILE_FLAGS) \
	-lpthread \
	$< -I src

setting.o: src/setting.cpp src/setting.h
	$(CC) -c $(COMPILE_FLAGS) \
	-lpthread \
	$< -I src

ui.o: src/ui.cpp src/ui.h
	$(CC) -c $(COMPILE_FLAGS) \
	-lpthread \
	$< -I src \
	$(shell $(abs_path)/bin/ncursesw6-config --cflags)

weapon.o: src/weapon.cpp src/weapon.h
	$(CC) -c $(COMPILE_FLAGS) $< -I src

zombie.o: src/zombie.cpp src/zombie.h 
	$(CC) -c $(COMPILE_FLAGS) \
	-lpthread \
	$< -I src

mainloop.o: src/mainloop.cpp src/mainloop.h
	$(CC) -c $(COMPILE_FLAGS) $< -I src

main.o: src/main.cpp
	$(CC) -c $(COMPILE_FLAGS) $< -I src

main: bullet.o map.o player.o setting.o ui.o weapon.o zombie.o mainloop.o main.o
	g++ $^ -o main \
	$(shell $(abs_path)/bin/ncursesw6-config --libs)

clean:
	rm map.o screen.o mainloop.o main.o
