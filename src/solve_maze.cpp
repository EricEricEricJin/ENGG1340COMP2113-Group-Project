#include <iostream>
#include <queue>
#include <string.h>

#include "solve_maze.h"

bool valid(char **map, int lines, int cols, char **searched, std::pair<int, int> p, char wall, char empty) // y, x
{
    if (p.first < 0 || p.first >= lines || p.second < 0 || p.second >= cols)
        return false;
    if (searched[p.first][p.second])
        return false;
    if (map[p.first][p.second] == wall)
        return false;
    return true;
};

int bfs(char **map, int lines, int cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty)
{
    char **searched = new char *[lines];
    for (int i = 0; i < lines; i++)
        searched[i] = new char[cols];
    for (int i = 0; i < lines; i++)
        memset(searched[i], 0, sizeof(char) * cols);

    std::queue<std::pair<int, int>> q;
    q.push(source_yx);

    for (int depth = 0; !q.empty(); depth++)
    {
        int layer_n = q.size();
        for (int i = 0; i < layer_n; i++)
        {
            // memset searched
            auto front = q.front();
            q.pop();

            if (front == target_yx)
            {
                delete searched;
                return depth;
            }
            else
            {
                // Add neighbours
                if (valid(map, lines, cols, searched, {front.first + 1, front.second}, wall, empty))
                {
                    q.push({front.first + 1, front.second});
                    searched[front.first + 1][front.second] = 1;
                }
                if (valid(map, lines, cols, searched, {front.first - 1, front.second}, wall, empty))
                {
                    q.push({front.first - 1, front.second});
                    searched[front.first - 1][front.second] = 1;
                };
                if (valid(map, lines, cols, searched, {front.first, front.second + 1}, wall, empty))
                {
                    q.push({front.first, front.second + 1});
                    searched[front.first][front.second + 1] = 1;
                }
                if (valid(map, lines, cols, searched, {front.first, front.second - 1}, wall, empty))
                {
                    q.push({front.first, front.second - 1});
                    searched[front.first][front.second - 1] = 1;
                }
            }
        }
    }
    delete searched;
    return -1;
}

int solve_maze(char **map, int lines, int cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty)
{
    int min_dir = -1;
    int min_step = 9999;
    std::pair<int, int> _source_yx;
    for (int dir : {SOLMAZ_UP, SOLMAZ_DOWN, SOLMAZ_LEFT, SOLMAZ_RIGHT})
    {
        _source_yx = source_yx;
        if (dir == SOLMAZ_UP)
            _source_yx.first--;
        else if (dir == SOLMAZ_DOWN)
            _source_yx.first++;
        else if (dir == SOLMAZ_LEFT)
            _source_yx.second--;
        else if (dir == SOLMAZ_RIGHT)
            _source_yx.second++;

        if ((_source_yx.first >= 0 && _source_yx.first < lines && _source_yx.second >= 0 && _source_yx.second < cols) && map[_source_yx.first][_source_yx.second] != wall)
        {
            // valid
            int temp_step = bfs(map, lines, cols, _source_yx, target_yx, wall, empty);
            // std::cout << temp_step << std::endl;
            if (temp_step != -1 && temp_step < min_step)
            {
                min_step = temp_step;
                min_dir = dir;
            }
        }
    }
    return min_dir;
}