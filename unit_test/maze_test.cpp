#include <iostream>
#include "solve_maze.h"

using namespace std;

char map[][79] = {
    "                                                                              ",
    "                                                                              ",
    "                                                                              ",
    "     ##########         ##############################         ##########     ",
    "     #                                                                  #     ",
    "     #                                                                  #     ",
    "     #                                                                  #     ",
    "                                                                              ",
    "                                                                              ",
    "                                                                              ",
    "                             ####################                             ",
    "                                                                              ",
    "                                                                              ",
    "                                                                              ",
    "     #                                                                  #     ",
    "     #                                                                  #     ",
    "     #                                                                  #     ",
    "     ##########         ##############################         ##########     ",
    "                                                                              ",
    "                                                                              ",
    "                                                                              ",
};

int main()
{
    char *a[21];
    for (int i = 0; i < 21; i++)
        a[i] = map[i];

    // solveMaze solver;
    // cout << solver.search(a, 21, 78, {3, 2}, {4, 6}, '#', ' ') << endl;
    cout << solve_maze(a, 21, 78, {12, 1}, {12, 39}, '#', ' ') << endl;
    return 0;
}