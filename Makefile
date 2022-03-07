abs_path = $(shell pwd)

.PHONY: install_ncurses uninstall_ncurses clean
install_ncurses:
	cd $(abs_path)/ncurses-6.3; \
	./configure --prefix=$(abs_path) --enable-widec; \
	make -j4; \
	make install.includes; \
	make install.data; \
	make install.libs; \

uninstall_ncurses:
	cd $(abs_path)/ncurses-6.3; \
	make uninstall; \
	make clean

map.o: src/map.h src/map.cpp
	g++ -c src/map.cpp -I src

screen.o: src/screen.h src/screen.cpp
	g++ -c src/screen.cpp \
	-I src \
	$(shell $(abs_path)/bin/ncursesw6-config --cflags)

mainloop.o: src/mainloop.h src/mainloop.cpp
	g++ -c src/mainloop.cpp \
	-I src

main.o: src/main.cpp
	g++ -c src/main.cpp \
	-I src

main: main.o map.o screen.o mainloop.o
	g++ $^ -o main \
	$(shell $(abs_path)/bin/ncursesw6-config --libs)

clean:
	rm map.o screen.o mainloop.o main.o
