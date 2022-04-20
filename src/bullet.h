#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <list>

#include "tinyexpr.h"
#include "clock.h"

#define TRIG_TIMER 1
#define TRIG_CONTACT 0

float pair_distance(std::pair<float, float> p1, std::pair<float, float> p2);

namespace game
{
    class Map;
    class Wall;
    class Player;
    class Zombie;

    class Bullet
    {
    private:
        std::string type;
        clock_tick_t shoot_time;
        std::pair<float, float> yx;
        int dir;

        std::string character;

    public:
        Bullet(std::string _type, clock_tick_t _shoot_time, std::pair<float, float> _yx, int _dir);
        std::pair<float, float> get_yx();
        std::string get_type();
        std::string get_char();
        clock_tick_t get_shoot_time();
    };

    struct bulletType
    {
        std::string chr;
        float speed;
        int trig_mode;
        float damage_dist;
        int trig_c;
        std::vector<std::string> trig_obj;

        te_expr *damage_func;
    };

    class bulletManager
    {
    private:
        Map *map;
        std::list<Zombie *> *zombie_list;
        Player *player;

        std::map<std::string, bulletType *> bul_type_dict;
        std::list<Bullet *> *bullet_list;
        bool running;

        void _thread_loop();
        std::thread *thread_obj;

        float temp_distance; // pass to tiny expr

        Clock *clock;

    public:
        bulletManager(Map *_map, std::list<Zombie *> *_zombie_list, Player *_player, Clock *_clock);

        int load_resource(std::string resource_root);
        void print();

        std::list<Bullet *> *get_bullet_list();

        void run();

        void shoot(std::string name, std::pair<int, int> yx, int dir); // add bullet to loop

        void stop();

        std::vector<std::string> get_names();

        ~bulletManager();
    };
}

#endif
