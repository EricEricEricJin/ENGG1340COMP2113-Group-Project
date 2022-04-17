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
        Zombie(std::vector<Bullet *> *bullet_list, Map *map, Player *player);
        std::pair<float, float> get_xy();
        std::string get_char();
        float get_hp();
        bool set_hp(float new_hp);
        void run();
        void stop();
        ~Zombie();
    };

    class zombieKing : public Zombie
    {
    };

    class zombieOrdinary : public Zombie
    {
    };
}
#endif