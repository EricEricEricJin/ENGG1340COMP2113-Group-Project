#include "screen.h"

Screen::Screen(int bird_x)
{
    _bird_x = bird_x;
    initscr();
    cbreak();
    timeout(0);
    noecho();
}

void Screen::set_bird_y(int *bird_y)
{
    _bird_y = bird_y;
}

void Screen::set_map(int **map, int width, int height)
{
    _map = map;
    _map_w = width;
    _map_h = height;
}

void Screen::update(int score)
{
    clear();
    for (int row = 0; row < _map_h; row++)
    {
        for (int col = 0; col < _map_w; col++)
        {
            if (*(*(_map + row) + col) == 1)
            {
                move(row, col);
                addch(WALL_PIX);
            }
        }
    }

    move(*_bird_y, _bird_x);
    addch(BIRD_PIX);

    move(_map_h - 2, _map_w - 6);
    char score_str[5];
    sprintf(score_str, "%d", score);
    addstr(score_str);

    refresh();
}

void Screen::die()
{
    // printf("HAMA");
    clear();
    move(_map_h / 2, _map_w / 2);
    addstr("DIE");
    refresh();
}