#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include "tinyexpr.h"

#define TRIG_TIMER 1
#define TRIG_CONTACT 0

float pair_distance(std::pair<float, float> p1, std::pair<float, float> p2);

namespace game
{
    class Map;
    class Wall;
    class Player;
    class Zombie;

    typedef uint64_t timer_t;

    class Bullet
    {
    private:
        std::string type;
        timer_t shoot_time;
        std::pair<float, float> yx;
        int dir;

    public:
        Bullet(std::string type, timer_t timer, std::pair<float, float> yx, int dir);
        std::pair<float, float> get_yx();
        std::string get_type();
        std::string get_char();
        timer_t get_shoot_time();
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
        std::vector<Zombie *> *zombie_list;
        Player *player;

        std::map<std::string, bulletType *> bul_type_dict;
        std::vector<Bullet *> bullet_list;
        bool running;

        void _thread_loop();
        std::thread *thread_obj;

        float temp_distance; // pass to tiny expr

        timer_t *timer;

    public:
        bulletManager();

        int load_resource(std::string resource_root);
        void print();

        std::vector<Bullet *> *get_bullet_list();

        void run(Map *_map, std::vector<Zombie *> *_zombie_list, Player *_player);

        void shoot(std::string name, std::pair<int, int> yx, int dir); // add bullet to loop

        void pause();
        void resume();

        std::vector<std::string> get_names();

        ~bulletManager();
    };
}

#endif
