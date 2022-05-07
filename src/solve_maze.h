#ifndef _SOLVE_MAZE_H
#define _SOLVE_MAZE_H

#include <iostream>

enum
{
    SOLMAZ_UP,
    SOLMAZ_DOWN,
    SOLMAZ_LEFT,
    SOLMAZ_RIGHT,
};

int solve_maze(char **map, int lines, int cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty);

#endif