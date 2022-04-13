#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include <vector>
#include <thread>

#define BUL_PATH "../resource/bullet/"

namespace game
{
    class Map;
    class Wall;
    class Player;
    class Zombie;

    struct Bullet
    {
        int type;
        std::string chr;
        int x;
        int y;
        int dir;
    };

    class bulletManager
    {
    private:
        Map *map;
        std::vector<Zombie *> *zombie_list;
        Player *player;

        std::vector<Bullet> bullet_list;

        // define a type "bullet_t"
        // map<string, bullet_t>
        std::map<std::string, std::string> char_dict;
        std::map<std::string, float> speed_dict;
        std::map<std::string, bool *()> trigger_dict;
        std::map<std::string, float *()> damage_dict;

        bool running;

        void _thread_loop();
        std::thread *thread_obj;

    public:
        bulletManager();

        int load_resource();
        void run(Map *_map, std::vector<Zombie *> *_zombie_list, Player *_player);

        void shoot(std::string name); // add bullet to loop

        void pause();
        void resume();

        std::string get_char();
        std::pair<int, int> get_xy();
        std::vector<std::string> get_names();

        ~bulletManager();
    };
}

#endif
