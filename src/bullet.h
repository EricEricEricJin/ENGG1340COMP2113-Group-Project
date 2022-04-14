#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <map>
#include "tinyexpr.h"

#define BUL_PATH "../resource/bullet/"

#define TRIG_TIMER 1
#define TRIG_CONTACT 0

float pair_distance(auto p1, auto p2)
{
    return ((p1.first - p2.first) * (p1.first - p2.first) +
            (p1.second - p2.second) * (p1.second - p2.second));
}

namespace game
{
    class Map;
    class Wall;
    class Player;
    class Zombie;

    typedef uint64_t timer_t;

    struct Bullet
    {
        std::string type;
        timer_t shoot_time;
        std::pair<float, float> xy;
        int dir;
    };

    struct bullet_t
    {
        std::string chr;
        float speed;
        int trig_mode;
        int damage_dist;
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

        std::vector<Bullet *> bullet_list;

        std::map<std::string, bullet_t *> bul_type_dict;
        bool running;

        void _thread_loop();
        std::thread *thread_obj;

        float temp_distance; // pass to tiny expr

        timer_t *timer;

    public:
        bulletManager();

        int load_resource(std::string resource_root);
        void run(Map *_map, std::vector<Zombie *> *_zombie_list, Player *_player);

        void shoot(std::string name, int x, int y, int dir); // add bullet to loop

        void pause();
        void resume();

        std::string get_char();
        std::pair<int, int> get_xy();
        std::vector<std::string> get_names();

        ~bulletManager();
    };
}

#endif
