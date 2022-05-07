#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <list>
#include <mutex>

#include "tinyexpr.h"
#include "clock.h"

#define TRIG_TIMER 1
#define TRIG_CONTACT 0

float pair_distance(std::pair<float, float> p1, std::pair<float, float> p2);

namespace game
{
    enum
    {
        BDIR_STOP,
        BDIR_UP,
        BDIR_DOWN,
        BDIR_LEFT,
        BDIR_RIGHT,
    };

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
        Bullet(std::string type, clock_tick_t shoot_time, std::pair<float, float> yx, int dir, std::string character);
        std::pair<float, float> get_yx();
        int get_dir();
        std::string get_type();
        std::string get_char();
        clock_tick_t get_shoot_time();
        void move(std::pair<float, float> new_yx);
    };

    struct bulletType
    {
        std::string chr;
        float speed;
        int trig_mode;
        float damage_dist;
        int trig_c;
        std::vector<std::string> trig_obj;
        std::vector<std::string> damage_obj;

        te_expr *damage_func;

        ~bulletType()
        {
            delete damage_func;
        }
    };

    class bulletManager
    {
    private:
        Map *map;
        std::list<Zombie *> *zombie_list;
        Player *player;
        Clock *clock;

        std::list<Bullet *> *bullet_list;
        std::map<std::string, bulletType *> bul_type_dict;

        bool running;
        bool paused;
        void _thread_loop();
        std::thread *thread_obj;
        std::mutex bullet_list_lock;

        double temp_distance; // pass to tiny expr

        void _out_of_map(std::pair<float, float> yx);

    public:
        bulletManager();
        void init(Map *map, std::list<Zombie *> *zombie_list, Player *player, Clock *clock);
        int load_resource(std::string resource_path);

        void set_variables(std::vector<std::string> types, std::vector<int> shoot_times, std::vector<std::pair<int, int>> yxs, std::vector<int> dirs);
        void get_variables(std::vector<std::string> &types, std::vector<int> &shoot_times, std::vector<std::pair<int, int>> &yxs, std::vector<int> &dirs);
        void print();

        void reset();
        void run();
        void shoot(std::string name, std::pair<float, float> yx, int dir, clock_tick_t shoot_time); // add bullet to loop
        void stop();
        void pause();
        void resume();

        std::vector<std::string> get_names();
        std::list<Bullet *> *get_bullet_list();

        ~bulletManager();
    };
}

#endif
