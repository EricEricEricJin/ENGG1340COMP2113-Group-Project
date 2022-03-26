#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include "zombie.h"
#include "map.h"

#define UP 0b00'01
#define DOWN 0b00'10
#define RIGHT 0b10'00
#define LEFT 0b01'00

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
        std::vector<int> get_xy();
        std::string get_char();
        int get_shooter();
        int get_life_remain();
        virtual void go();
        virtual int damage(int distance);
        virtual bool triggered();
        ~Bullet();
    };

    class bullet9mm : public Bullet // for machine gun and pistol
    {
    private:
        std::vector<Zombie> *zombie_list;
        std::vector<Wall> *wall_list;
        const int damage_c = 0;
    public:
        void init(std::vector<Zombie> &_zombie_list, std::vector<Wall> &_wall_list);
        int damage(int distance);
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