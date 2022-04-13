//Control the movement of the player using the arrow keys of the keyboard//
#include <iostream>
#include <ncueses.h>
#include <string>
 
using namespace std;
 
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_X 120

struct Point{
        int x;
        int y;
    };
 int game_board_size = 10;//size of game board//   
int main(){
    
    Point position;
    position ={0,0};
    char key = getch();
    int value = key;
 
    while(value != KEY_X){
        
        switch(getch()){
            
        case KEY_UP:
            if (position.y>0)
            position.y--;
            break;
        case KEY_DOWN:
            if (position.y<game_board_size)
            position.y++;
            break;
        case KEY_LEFT:
            if (position.x>0)
            position.x--;
            break;
        case KEY_RIGHT:
            if (position.x<game_board_size)
            position.x++;
            break;
        }
 
        key = getch();
        value = key;
 
    }
 
    system("pause");
    return 0;
}
