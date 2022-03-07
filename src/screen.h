#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define WALL_PIX 42
#define BIRD_PIX 62

class Screen
{
public:
    Screen(int bird_x);
    void set_bird_y(int *bird_y);
    int get_width();
    int get_height();
    void set_map(int **map, int width, int height);
    void update(int score);
    void die();

private:
    int _bird_x;
    int *_bird_y;
    int **_map;
    int _map_w;
    int _map_h;
};