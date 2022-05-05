#include <iostream>
#include <ncurses.h>

using namespace std;

int main()
{
    initscr();
    cbreak();
    timeout(-1);
    echo();

    char s[100] = {0};
    mvwgetnstr(stdscr, 10, 10, s, 8);
    mvwprintw(stdscr, 12, 10, "%s", s);

    getch();
    endwin();
    return 0;
}