#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <iostream>
#include <vector>
#include <list>

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
        std::string character;

    public:
        Zombie(std::pair<int, int> yx, int _type);
        int get_type();
        float get_hp();
        bool set_hp(float new_hp);
        std::pair<float, float> get_yx();
        void move(std::pair<float, float> new_yx);
        int get_damage();
        float get_speed();
        std::string get_char();
    };

    class zombieManager
    {
    private:
        std::vector<Bullet *> *bullet_list;
        Map *map;
        Player *player;

        // std::vector<Zombie *> *zombie_list;
        std::list<Zombie*> *zombie_list;

        bool running;
        void _thread_loop();

    public:
        zombieManager(std::vector<Bullet *> *_bullet_list, Map *_map, Player *_player);

        void add(int type, std::pair<int, int> yx);
        std::list<Zombie*> *get_zombie_list();

        int get_num(); // number of zombies alive
        void run();
        void stop();
        ~zombieManager();
    };

}
#endif