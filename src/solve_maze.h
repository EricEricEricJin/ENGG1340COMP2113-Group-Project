#include <iostream>
#include <queue>
#include <string.h>

class solveMaze
{
private:
    char **searched;
    char **map;
    std::pair<int, int> Dest;

    int lines;
    int cols;

    char WALL;
    char EMPTY;

    bool valid(std::pair<int, int> p);
    int bfs_nstep(std::queue<std::pair<int, int>> *root);

public:;
    int search(char **_map, int _lines, int _cols, std::pair<int, int> source_yx, std::pair<int, int> target_yx, char wall, char empty);
};
