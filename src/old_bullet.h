#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <thread>

#ifndef UP
#define UP 0b11'10
#define DOWN 0b11'01
#define LEFT 0b10'11
#define RIGHT 0b01'11
#endif

namespace game
{
    class Map;
    class Wall;
    class Player;
    class Zombie;

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
        bool running;

        virtual void _thread_loop();

    public:
        Bullet();
        virtual void init();
        std::pair<int, int> get_xy();
        std::string get_char();
        int get_shooter();
        // int get_life_remain();
        void run();
        void pause();
        
        virtual float damage(float distance);
        ~Bullet();
    };

    class bullet9mm : public Bullet // for machine gun and pistol
    {
    private:
        std::vector<Zombie *> *zombie_list;
        std::vector<Wall *> *wall_list;
        Map *map;
        const float damage_c = 0;
        void _thread_loop();
    public:
        void init(std::vector<Zombie *> *_zombie_list, std::vector<Wall *> *_wall_list, int _x, int _y, int _dir);
        float damage(float distance);
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
