#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <list>
#include <thread>

#include <unistd.h>
#include <sstream>

#include "clock.h"

namespace game
{
    class Player;
    class Zombie;
    class Map;
    class Bullet;

    class UI
    {
    private:
        char mode;

        Player *player;
        std::list<Zombie *> *zombie_list;
        std::list<Bullet *> *bullet_list;
        Map *map;
        Clock *clock;

        int status_val;
        std::thread *thread_obj;

        WINDOW *game_win;
        WINDOW *status_win;

        int key;

        void _game_thread_loop();
        void _game_menu();

        // void _bottom_mode();

    public:
        UI();
        void init(Player *_player, std::list<Zombie *> *_zombie_list, std::list<Bullet *> *_bullet_list, Map *_map, Clock *_clock);
        int* get_key_ptr();
        bool homepage(std::string *map_name, int *difficulty); // return only when select map and mode
        // parameter: [map list: Map]
        // return value: [map id: int], [# of players: int], [difficulty: int]

        void start_game();
        void stop_game();

        int get_status();

        void exit_game();

        ~UI();
    };
}

#endif