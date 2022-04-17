#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player(std::vector<Bullet *> *bullet_list, Map *map)
    {
    }

    float Player::get_hp()
    {
        return hp;
    }

    bool Player::set_hp(float new_hp)
    {
        hp = new_hp;
        if (hp > 0)
            return true;
        else
            return false;
    }

    std::string Player::get_char()
    {
        return chr;
    }

    std::pair<float, float> Player::get_yx()
    {
        return {y, x};
    }

    void Player::run()
    {
        running = true;
        thread_obj = new std::thread([=]
                                     { _player_thread_loop(); });
    }

    void Player::_player_thread_loop()
    {
        int key;
        while (running)
        {
            key = getch();
            if (key == _KEY_UP)
                direction = DIR_UP;
            else if (key == _KEY_DOWN)
                direction = DIR_DOWN;
            else if (key == _KEY_LEFT)
                direction = DIR_LEFT;
            else if (key == _KEY_RIGHT)
                direction = DIR_RIGHT;
            else if (key == _KEY_STOP)
                direction = DIR_STOP;

            if (direction == DIR_UP)
                y -= speed;
            else if (direction == DIR_DOWN)
                y += speed;
            else if (direction == DIR_LEFT)
                x -= speed;
            else if (direction == DIR_RIGHT)
                x += speed;

            // sleep, using conditional variable 'wait()'
        }
    }

    void Player::stop()
    {
        running = false;
        thread_obj->join();
        delete thread_obj;
    }

    Player::~Player() {}
}