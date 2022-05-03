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
        UTHEME_LIGHT = 0b0001,
        UTHEME_DARK = 0b0010,
        UTHEME_THIN = 0b0100,
        UTHEME_BORD = 0b1000
    };

    enum
    {
        UCOLOR_BOX = 1,
        UCOLOR_MENU,
        UCOLOR_PLAYER,
        UCOLOR_ZOMBIE,
        UCOLOR_WALL,
        UCOLOR_BULLET,
        UCOLOR_HP_H,
        UCOLOR_HP_M,
        UCOLOR_HP_L
    };

    enum
    {
        USTATUS_RUNNING = 0,
        USTATUS_PAUSE = 1,
        USTATUS_EXIT = 2
    };
    enum
    {
        HOMEPAGE_NEWG = 0,
        HOMEPAGE_LOAD = 1,
        HOMEPAGE_SETT = 2,
        HOMEPAGE_EDIT = 3,
        HOMEPAGE_EXIT = 4
    };

    struct uiKeySet
    {
        int key_up;
        int key_down;
        int key_enter;
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
        uiKeySet keyset;

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

        int theme;

        void _game_thread_loop();
        int _game_menu();

        bool _new_game_page(std::string *map_name); // map name
        std::string _load_saving_page();            // saving name
        void _setting_page();
        void _edit_map_page();
        // void _bottom_mode();

    public:
        UI();

        bool init(Player *player, std::list<Zombie *> *zombie_list, std::list<Bullet *> *bullet_list, Map *map, Clock *clock);
        void configure(uiKeySet keyset, int theme);

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