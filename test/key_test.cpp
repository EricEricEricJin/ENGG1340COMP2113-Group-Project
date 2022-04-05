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

    int key = 0;
    vector<int> key_list = {0, 0}; 
    while (key_list[0] != '\n')
    {
        key_list = {0, 0};
        for (auto& k : key_list)
            k = getch();
        clear();   
        addstr((to_string(key_list[0]) + " " + to_string(key_list[1])).c_str());
    }

    endwin();
    return 0;
}