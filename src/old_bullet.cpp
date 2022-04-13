#include "bullet.h"

#include "zombie.h"
#include "player.h"
#include "map.h"

namespace game
{

    Bullet::Bullet()
    {
    }

    std::pair<int, int> Bullet::get_xy()
    {
        std::pair<int, int> ret = {(int)x, (int)y};
        return ret;
    }

    std::string Bullet::get_char()
    {
        return character;
    }

    int Bullet::get_shooter()
    {
        return shooter;
    }

    void Bullet::run()
    {
        running = true;
        std::thread(_thread_loop);
    }

    void Bullet::pause()
    {
        running = false;
    }

    Bullet::~Bullet()
    {
    }

    void bullet9mm::init(std::vector<Zombie *> *_zombie_list, std::vector<Wall *> *_wall_list, int _x, int _y, int _dir)
    {
        zombie_list = _zombie_list;
        wall_list = _wall_list;
        x = _x;
        y = _y;
        dir = _dir;
    }

    float bullet9mm::damage(float distance)
    {
        if (distance)
            return 0;
        return damage_c;
    }

    void bullet9mm::_thread_loop()
    {
        while (running)
        {
            // run and detect trigger
        }
    }

}