#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include "zombie.h"
#include "map.h"

#ifndef UP
#define UP 0b11'10
#define DOWN 0b11'01
#define LEFT 0b10'11
#define RIGHT 0b01'11
#endif

namespace game
{
    class Bullet
    {
    protected:
        float x;
        float y;
        int dir;
        float speed;
        int life_remain;
        int range;
        int shooter;
        std::string character;

    public:
        Bullet();
        virtual void init();
        std::pair<int, int> get_xy();
        std::string get_char();
        int get_shooter();
        // int get_life_remain();
        virtual void run();
        virtual void stop();
        virtual float damage(float distance);
        virtual bool triggered();
        ~Bullet();
    };

    class bullet9mm : public Bullet // for machine gun and pistol
    {
    private:
        std::vector<Zombie> *zombie_list;
        std::vector<Wall> *wall_list;
        const float damage_c = 0;

    public:
        void init(std::vector<Zombie> &_zombie_list, std::vector<Wall> &_wall_list, int _x, int _y, int _dir);
        float damage(float distance);
        bool triggered();
    };

    class bulletRPG : public Bullet
    {
    };

    class bulletGrenade : public Bullet
    {
    };

    class bulletShotgun : public Bullet
    {
    };

}

#endif
