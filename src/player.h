#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <ncurses.h>
#include <thread>
#include <vector>

namespace game
{
    class Map;
    class Bullet;

    class Player
    {
    private:
        /* data */

        const int DIR_STOP = 0;
        const int DIR_UP = 1;
        const int DIR_DOWN = 2;
        const int DIR_LEFT = 3;
        const int DIR_RIGHT = 4;

        int _KEY_UP;
        int _KEY_DOWN;
        int _KEY_LEFT;
        int _KEY_RIGHT;
        int _KEY_STOP;

        int direction;
        float speed;

        float hp;
        std::string chr;
        float x, y;

        void _player_thread_loop();
        std::thread* thread_obj;
        bool running;

    public:
        Player(std::vector<Bullet *> *bullet_list, Map *map);
        float get_hp();            // return hp
        bool set_hp(float new_hp); // true for alive, false for dead
        std::string get_char();    // depend on direction
        std::pair<float, float> get_yx();
        void run();
        void stop();
        // void set_hp(int new_hp); // set hp
        ~Player();
    };

}

#endif
