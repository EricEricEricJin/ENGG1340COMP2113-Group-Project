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
    enum
    {
        HOMEPAGE_NEWG = 0,
        HOMEPAGE_LOAD = 1,
        HOMEPAGE_SETT = 2,
        HOMEPAGE_EDIT = 3,
        HOMEPAGE_EXIT = 4
    };

    class Player;
    class Zombie;
    class Map;
    class Bullet;

    class UI
    {
    private:
        const int WIN_WIDTH = 80, WIN_HEIGHT = 24;
        int WIN_OFFSET_Y, WIN_OFFSET_X;

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

        bool _new_game_page(std::string *map_name);    // map name
        std::string _load_saving_page(); // saving name
        void _setting_page();
        void _edit_map_page();
        // void _bottom_mode();

    public:
        UI();
        void init(Player *_player, std::list<Zombie *> *_zombie_list, std::list<Bullet *> *_bullet_list, Map *_map, Clock *_clock);
        int *get_key_ptr();
        bool homepage(std::string *ret_string, int *ret_kind); // return only when select map and mode
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