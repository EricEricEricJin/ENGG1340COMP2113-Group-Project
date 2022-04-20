#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <thread>

#include <unistd.h>
#include <sstream>

#include "clock.h"


#define MAX_DIFF_LEVEL 10

#define UI_PAUSE 0
#define UI_QUIT 1

#define STATUS_RUNNING 1
#define STATUS_MENU 2

#define MENU_KEY ((int)'p')

namespace game
{
    class Player;
    class Zombie;
    class Map;
    class Bullet;

    class Block
    {
    private:
    public:
        Block();
        ~Block();
    };

    class UI
    {
    private:
        char mode;

        Player *player;
        std::list<Zombie *> *zombie_list;
        std::list<Bullet *> *bullet_list;
        Map *map;
        Clock* clock;

        int status_val;
        std::thread* thread_obj;

        void _game_thread_loop();

        void _game_menu();
        void _bottom_mode();

        void _draw_walls();
        void _draw_players();
        void _draw_zombies();
        void _draw_bullets();
        void _show_info(); // Display score, weapon, etc

    public:
        UI(Player *_player, std::list<Zombie*> *_zombie_list, std::list<Bullet*> *_bullet_list, Map *_map, Clock* _clock);
        void init();
        bool homepage(std::string* map_name, int *difficulty); // return only when select map and mode
        // parameter: [map list: Map]
        // return value: [map id: int], [# of players: int], [difficulty: int]

        void start_game();
        void stop_game();
        // THIS FUNC IS NON-BLOCK
        // Will start UI in new thread

        int get_status();

        int status();
        // modify parameters with pointer

        void exit_game();
        // Force stop the game threading

        ~UI();
    };
}
#endif

/*
what to do in mainloop:
loop
    start_homepage()
    start_game()
    loop
        get_op()
        if not running
            break
        endif
        do ...
    endloop
endloop
*/