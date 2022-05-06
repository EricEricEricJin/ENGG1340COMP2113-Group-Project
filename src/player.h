#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <ncurses.h>
#include <thread>
#include <vector>
#include <list>
#include "clock.h"

namespace game
{
    struct playerKeySet
    {
        int UP = 1;
        int DOWN = 2;
        int LEFT = 3;
        int RIGHT = 4;
        int STOP = 0;
        int FIRE = 5;
    };

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

        int *key_ptr;

        int direction;

        float speed;

        std::string cur_bul_name;

        playerKeySet *key_set;

        float hp;
        std::string chr = "P";
        float x = -1, y = -1;

        void _player_thread_loop();
        std::thread *thread_obj;
        bool running;
        bool paused;

        bool _debug;

    public:
        Player();
        void init(bulletManager *bullet_manager, Map *map, Clock *clock, int *key_ptr);
        
        void set_variables(std::pair<int, int> yx, int hp, int dir, std::string cur_bul_name);
        void get_variables(std::pair<int, int>& yx, int& hp, int& dir, std::string& cur_bul_name);

        void configure(playerKeySet new_keyset);

        float get_hp();            // return hp
        bool set_hp(float new_hp); // true for alive, false for dead
        chtype get_char();    // depend on direction
        std::pair<float, float> get_yx();

        void reset();
        void run(bool debug = false);
        void stop();

        void pause();
        void resume();

        std::string get_cur_bul_name();
        ~Player();
    };

}

#endif
