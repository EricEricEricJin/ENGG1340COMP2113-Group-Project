#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <ncurses.h>
#include <vector>

#include "player.h"
#include "map.h"
#include "zombie.h"
#include "bullet.h"

#ifndef MODE_GAME
#define MODE_SETTING 0
#define MODE_GAME 1
#endif

namespace game
{
    class UI
    {
    private:

        char mode;

        std::vector<Player> *player_list;
        std::vector<Zombie> *zombie_list;
        std::vector<Bullet> * bullet_list;
        Map *map;

        void _draw_walls();
        void _draw_players();
        void _draw_zombies();
        void _draw_bullets();

    public:
        UI();
        void start_game(std::vector<Player> &_player_list, std::vector<Zombie> &_zombie_list, Map &_map);
        void refresh();
        ~UI();
    };

}
#endif