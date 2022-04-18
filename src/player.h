#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <ncurses.h>
#include <thread>
#include <vector>
#include "clock.h"

struct playerKeySet
{
    int UP;
    int DOWN;
    int LEFT;
    int RIGHT;
    int STOP;
    int FIRE;
};

namespace game
{
    enum
    {
        PDIR_STOP,
        PDIR_UP,
        PDIR_DOWN,
        PDIR_LEFT,
        PDIR_RIGHT,
    };

    class Map;
    class Bullet;
    class bulletManager;

    class Player
    {
    private:
        Map *map;
        bulletManager *bullet_manager;

        Clock *clock;

        int direction;

        float speed;

        std::string cur_bul_name;

        playerKeySet *key_set;

        float hp;
        std::string chr;
        float x, y;

        void _player_thread_loop();
        std::thread *thread_obj;
        bool running;

    public:
        Player();
        void init(bulletManager *_bullet_manager, Map *_map, Clock *_clock);

        void configure(playerKeySet new_keyset);

        float get_hp();            // return hp
        bool set_hp(float new_hp); // true for alive, false for dead
        std::string get_char();    // depend on direction
        std::pair<float, float> get_yx();
        void run();
        void stop();
        ~Player();
    };

}

#endif
