#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <iostream>
#include <vector>

class Bullet;
class Player;
class Map;

namespace game
{

    class Zombie
    {
    private:
        float x;
        float y;
        std::string character;

    public:
        Zombie(std::vector<Bullet *> &bullet_list, Map &map, Player &player);
        std::pair<int, int> get_xy();
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