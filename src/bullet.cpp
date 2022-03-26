#include "bullet.h"

game::Bullet::Bullet()
{
}

std::vector<int> game::Bullet::get_xy()
{
    std::vector<int> ret = {(int)x, (int)y};
    return ret;
}

std::string game::Bullet::get_char()
{
    return character;
}

int game::Bullet::get_shooter()
{
    return shooter;
}

int game::Bullet::get_life_remain()
{
    return life_remain;
}

void game::Bullet::go()
{
    switch (dir)
    {
    case UP:
        y -= speed;
        break;
    case (UP & RIGHT):
        y -= speed;
        x += speed;
        break;
    case RIGHT:
        x += speed;
        break;
    case (DOWN & RIGHT):
        x += speed;
        y += speed;
        break;
    case DOWN:
        y += speed;
        break;
    case (LEFT & DOWN):
        x -= speed;
        y += speed;
        break;
    case LEFT:
        x -= speed;
        break;
    case (LEFT & UP):
        x -= speed;
        y -= speed;
    default:
        break;
    }
}

game::Bullet::~Bullet()
{
}

void game::bullet9mm::init(std::vector<Zombie> &_zombie_list, std::vector<Wall> &_wall_list)
{
    zombie_list = &_zombie_list;
    wall_list = &_wall_list;
}

int game::bullet9mm::damage(int distance)
{
    if (distance)
        return 0;
    return damage_c;
}

bool game::bullet9mm::triggered()
{
    for (Zombie zombie : *zombie_list)
    {
        if (zombie.get_xy() == get_xy())
            goto TRIGGERED;
    }

    for (Wall wall : *wall_list)
    {
        if (wall.get_xy() == get_xy())
            goto TRIGGERED;
    }

    return false;

TRIGGERED:
    life_remain--;
    return true;
}