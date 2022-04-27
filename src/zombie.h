#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <iostream>
#include <vector>
#include <list>
#include "clock.h"

enum
{
    ZOMBIETYPE_ODNR,
    ZOMBIETYPE_KING
};

namespace game
{

    class Bullet;
    class Player;
    class Map;

    class Zombie
    {
    private:
        int type;
        float x;
        float y;
        float hp;
        float speed;
        float damage;
        std::string character = "*";

    public:
        clock_tick_t last_dmg_time;
        clock_tick_t dmg_delay;

        Zombie(std::pair<int, int> yx, int _type, clock_tick_t _dmg_delay);
        int get_type();
        float get_hp();
        bool set_hp(float new_hp);
        std::pair<float, float> get_yx();
        void move(std::pair<float, float> new_yx);
        float get_damage();
        float get_speed();
        std::string get_char();
    };

    class zombieManager
    {
    private:
        Clock *clock;

        std::list<Bullet *> *bullet_list;
        Map *map;
        Player *player;

        std::list<Zombie *> *zombie_list;
        // std::vector<Zombie *> *zombie_list;

        bool running;
        void _thread_loop();

        std::thread *thread_obj;

    public:
        zombieManager();
        void init(std::list<Bullet *> *_bullet_list, Map *_map, Player *_player, Clock *_clock);

        void add(int type, std::pair<int, int> yx);
        std::list<Zombie *> *get_zombie_list();

        int get_num(); // number of zombies alive
        void run();
        void stop();
        ~zombieManager();
    };

}
#endif