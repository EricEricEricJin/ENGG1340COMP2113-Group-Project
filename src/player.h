#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <ncurses.h>
#include <thread>

#include "weapon.h"
#include "bullet.h"
#include "map.h"

namespace game
{
    class Player
    {
    private:
        /* data */
        void _player_thread_loop();
        bool running;

    public:
        Player(std::vector<Bullet> &bullet_list, Map &map);
        float get_hp();            // return hp
        bool set_hp(float new_hp); // true for alive, false for dead
        std::string get_char();    // depend on direction
        std::pair<int, int> get_xy();
        Weapon *get_weapon();
        void run();
        void stop();
        // void set_hp(int new_hp); // set hp
        ~Player();
    };

}

#endif