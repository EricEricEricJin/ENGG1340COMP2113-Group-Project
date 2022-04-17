#include "zombie.h"

#include "bullet.h"
#include "player.h"
#include "map.h"

namespace game
{
    Zombie::Zombie(std::vector<Bullet *> *bullet_list, Map *map, Player *player)
    {
    }

    std::pair<float, float> Zombie::get_xy()
    {
        return {x, y};
    }

    std::string Zombie::get_char()
    {
        return character;
    }

    float Zombie::get_hp()
    {
        return hp;
    }

    bool Zombie::set_hp(float new_hp)
    {
        hp = new_hp;
        if (hp > 0)
            return true;
        return false;
    }

    void Zombie::run() {}

    void Zombie::stop() {}

    Zombie::~Zombie() {}
}