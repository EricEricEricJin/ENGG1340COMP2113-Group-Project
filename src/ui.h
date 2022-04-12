#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <thread>

#include <unistd.h>
#include <sstream>

// #include <experimental/filesystem>

#include "player.h"
#include "map.h"
#include "zombie.h"
#include "bullet.h"

#define MAX_DIFF_LEVEL 10

#define UI_PAUSE 0
#define UI_QUIT 1

#define STATUS_RUNNING 1
#define STATUS_MENU 2

#define MENU_KEY ((int)'p')

namespace game
{

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
        std::vector<Zombie> *zombie_list;
        std::vector<Bullet> *bullet_list;
        Map *map;

        int status_val;

        void _game_thread_loop();

        void _game_menu();

        void _draw_walls();
        void _draw_players();
        void _draw_zombies();
        void _draw_bullets();
        void _show_info(); // Display score, weapon, etc

    public:
        UI();
        bool homepage(std::vector<std::pair<std::string **, std::pair<int, int>>> maps, int &map_id, int &difficulty); // return only when select map and mode
        // parameter: [map list: Map]
        // return value: [map id: int], [# of players: int], [difficulty: int]

        void start_game(Player *player, std::vector<Zombie> *_zombie_list, Map *_map);
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