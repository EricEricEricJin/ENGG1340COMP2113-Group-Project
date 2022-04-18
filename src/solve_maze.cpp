// Use BFS
// Tested. Since map is small, speed is OK

#include <iostream>
#include <queue>
#include <string.h>
#include <functional>

#include "solve_maze.h"

int solve_maze(char **map, int lines, int cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty)
{
    char **searched = new char *[lines];
    for (int i = 0; i < lines; i++)
        searched[i] = new char[cols];

    auto valid = [map, lines, cols, wall, empty, searched](std::pair<int, int> p) // y, x
    {
        if (p.first < 0 || p.first >= lines || p.second < 0 || p.second >= cols)
            return false;
        if (searched[p.first][p.second])
            return false;
        if (map[p.first][p.second] == wall)
            return false;
        return true;
    };

    std::function<int(std::queue<std::pair<int, int>> *)> bfs_nstep;

    bfs_nstep = [valid, map, lines, cols, target_yx, searched, &bfs_nstep](std::queue<std::pair<int, int>> *root)
    {
        if (root->size() == 0)
            return -1;
        auto front = root->front();
        root->pop();

        if (front == target_yx)
            return 0;

        if (valid({front.first + 1, front.second}))
        {
            root->push({front.first + 1, front.second});
            searched[front.first + 1][front.second] = 1;
        }

        if (valid({front.first - 1, front.second}))
        {
            root->push({front.first - 1, front.second});
            searched[front.first - 1][front.second] = 1;
        }

        if (valid({front.first, front.second + 1}))
        {
            root->push({front.first, front.second + 1});
            searched[front.first][front.second + 1] = 1;
        }

        if (valid({front.first, front.second - 1}))
        {
            root->push({front.first, front.second - 1});
            searched[front.first][front.second - 1] = 1;
        }

        auto ret = bfs_nstep(root);
        if (ret == -1)
            return -1;
        return ret + 1;
    };

    int dy, dx;
    int min_step = 9999;
    int min_dir = -1;
    int temp_step;

    for (int dir : {SOLMAZ_UP, SOLMAZ_DOWN, SOLMAZ_LEFT, SOLMAZ_RIGHT})
    {
        if (dir == SOLMAZ_DOWN)
        {
            dy = 1;
            dx = 0;
        }
        else if (dir == SOLMAZ_UP)
        {
            dy == -1;
            dx = 0;
        }
        else if (dir == SOLMAZ_RIGHT)
        {
            dy = 0;
            dx = 1;
        }
        else if (dir == SOLMAZ_LEFT)
        {
            dy = 0;
            dx = -1;
        }

        for (int i = 0; i < lines; i++)
            memset(searched[i], 0, sizeof(char) * cols);

        if (!valid({source_yx.first + dy, source_yx.second + dx}))
        {
            continue;
        }

        auto root = new std::queue<std::pair<int, int>>;
        root->push({source_yx.first + dy, source_yx.second + dx});
        temp_step = bfs_nstep(root);
        delete root;

        if (temp_step == -1)
            continue; // no path

        else if (temp_step < min_step)
        {
            min_step = temp_step;
            min_dir = dir;
        }
    }
    delete searched;

    return min_dir;
}