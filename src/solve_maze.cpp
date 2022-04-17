#include "solve_maze.h"

bool solveMaze::valid(std::pair<int, int> p)
{
    if (p.first < 0 || p.first >= lines || p.second < 0 || p.second >= cols)
        return false;
    std::cout << "bound pass" << std::endl;
    std::cout << "searched[i][j]: " << (int)(searched[p.first][p.second]) << std::endl;
    if (searched[p.first][p.second])
        return false;
    std::cout << "searched pass" << std::endl;
    if (map[p.first][p.second] == WALL)
        return false;
    std::cout << "map pass" << std::endl;
    return true;
}

int solveMaze::bfs_nstep(std::queue<std::pair<int, int>> *root)
{
    if (root->size() == 0)
        return -1;
    auto front = root->front();
    root->pop();

    if (front == Dest)
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
}

int solveMaze::search(char **_map, int _lines, int _cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty)
{
    WALL = wall;
    EMPTY = empty;

    map = _map;

    lines = _lines;
    cols = _cols;
    Dest = target_yx;

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < cols; j++)
            std::cout << map[i][j];
        std::cout << std::endl;
    }

    std::queue<std::pair<int, int>> *root;

    // searched = (char **)malloc(sizeof(char[lines][cols]));
    searched = new char *[lines];
    for (int i = 0; i < cols; i++)
        searched[i] = new char[cols];

    int dy, dx;
    int min_step = 9999;
    int min_dir = -1;
    int temp_step;

    for (int dir = 0; dir < 4; dir++)
    {
        if (dir == 0)
        {
            dy = 1;
            dx = 0;
        }
        else if (dir == 1)
        {
            dy == -1;
            dx = 0;
        }
        else if (dir == 2)
        {
            dy = 0;
            dx = 1;
        }
        else if (dir == 3)
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

        root = new std::queue<std::pair<int, int>>;
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
    return min_dir;
}

solveMaze::~solveMaze()
{
    // for (int i = 0; i < lines; i++)
    //     delete searched[i];
    delete searched;
}