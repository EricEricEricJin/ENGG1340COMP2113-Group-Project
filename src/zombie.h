#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <iostream>
#include <vector>

namespace game
{

    class Bullet;
    class Player;
    class Map;

    class Zombie
    {
    private:
        float x;
        float y;
        float hp;
        std::string character;

    public:
        float get_hp();
        bool set_hp(float new_hp);
        std::pair<float, float> get_xy();
        std::string get_char();
    };

    class zombieManager
    {
    private:
        std::vector<Bullet *> *bullet_list;
        Map *map;
        Player *player;

        std::vector<Zombie *> *zombie_list;

        void _thread_loop();

    public:
        zombieManager(std::vector<Bullet *> *_bullet_list, Map *_map, Player *_player);

        void add(int type, std::pair<int, int> xy);
        std::vector<Zombie*> get_zombie_list();

        int get_num(); // number of zombies alive
        void run();
        void stop();
        ~zombieManager();
    };

}
#endif