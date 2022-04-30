#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
    
    initscr();
    nocbreak();
    noecho();

    timeout(20);

    int key = 20;
    while (key != '\n')
    {
        // key = getch();
        clear();   
        mvprintw(2, 2, "%d", key);
    }

    endwin();
    return 0;
}
