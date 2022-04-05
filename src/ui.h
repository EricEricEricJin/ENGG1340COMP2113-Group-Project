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

#define MAX_PLAYER_NUM 2
#define MAX_DIFF_LEVEL 10

#define UI_PAUSE 0
#define UI_QUIT 1

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

        std::vector<Player> *player_list;
        std::vector<Zombie> *zombie_list;
        std::vector<Bullet> *bullet_list;
        Map *map;

        bool running;

        void _game_thread_loop();

        void _draw_walls();
        void _draw_players();
        void _draw_zombies();
        void _draw_bullets();
        void _show_score(std::vector<int> scores);

    public:
        UI();
        bool homepage(std::vector<std::pair<std::string **, std::pair<int, int>>> maps, int &map_id, int &difficulty); // return only when select map and mode
        // parameter: [map list: Map]
        // return value: [map id: int], [# of players: int], [difficulty: int]

        void start_game(Player &player, std::vector<Zombie> &_zombie_list, Map &_map);
        // THIS FUNC IS NON-BLOCK
        // Will start UI in new thread

        int get_status();

        void get_op(bool &running, int &p1_dir, int &p1_shooting, int &p1_weapon, int &p2_dir, int &p2_shooting, int &p2_weapon); // [direction: int], [isshooting: bool], [weapon]
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