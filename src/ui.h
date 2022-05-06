#ifndef _UI_H
#define _UI_H

#include <iostream>

#include <ncurses.h>
// #include <menu.h>

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
        UNOTICE_NORMAL,
        UNOTICE_WARNING,
        UNOTICE_ERROR
    };

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
        HOMEPAGE_NEWG,
        HOMEPAGE_LOAD,
        HOMEPAGE_DELE,
        HOMEPAGE_EXIT,
    };

    struct uiKeySet
    {
        int key_up;
        int key_down;
        int key_enter;
        int key_quit;
    };

    class Player;
    class Zombie;
    class Map;
    class Bullet;
    class rwSaved;

    class UI
    {
    private:
        const std::string PLAYER_CHR = "P";
        const std::string ZOMBIE_CHR = "Z";

        const int WIN_WIDTH = 80, WIN_HEIGHT = 24;
        int WIN_OFFSET_Y, WIN_OFFSET_X;

        char mode;
        uiKeySet keyset;

        Player *player;
        std::list<Zombie *> *zombie_list;
        std::list<Bullet *> *bullet_list;
        Map *map;
        Clock *clock;
        rwSaved *rw_saved;

        int status_val;
        std::thread *thread_obj;

        WINDOW *game_win;
        WINDOW *status_win;

        int key;

        int theme;

        int _select_list(std::string prompt, std::vector<std::string> option_list, int height, int width, int y_beg, int x_beg, int* item_ptr = nullptr);
        std::string _prompt_input(std::string prompt, int max_len);

        void _game_thread_loop();

        bool _new_game_page(std::string *map_name, int* difficulty);      // map name
        bool _load_saving_page(std::string *ret_string); // saving name
        bool _delete_saving_page(std::string* ret_string);
        // void _setting_page();
        // void _edit_map_page();
        // void _bottom_mode();

    public:
        UI();

        bool init(Player *player, std::list<Zombie *> *zombie_list, std::list<Bullet *> *bullet_list, Map *map, Clock *clock, rwSaved *rw_saved);
        void configure(uiKeySet keyset, int theme);

        int *get_key_ptr();
        bool homepage(std::string *ret_string, int* ret_val, int *ret_kind); // return only when select map and mode
        // parameter: [map list: Map]
        // return value: [map id: int], [# of players: int], [difficulty: int]

        void start_game();
        void stop_game();

        int get_status();

        void exit_game();

        void notice(int type, std::string content);

        ~UI();
    };
}

#endif