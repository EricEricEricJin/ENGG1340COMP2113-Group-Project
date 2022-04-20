#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player(bulletManager *_bullet_manager, Map *_map, Clock *_clock)
    {
        bullet_manager = _bullet_manager;
        map = _map;
        clock = _clock;

        // default keysey
        key_set = new playerKeySet{'w', 's', 'a', 'd', 'e', ' '};
    }

    void Player::configure(playerKeySet new_keyset)
    {
        *key_set = new_keyset;
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
            if (key == key_set->UP)
                direction = PDIR_UP;
            else if (key == key_set->DOWN)
                direction = PDIR_DOWN;
            else if (key == key_set->LEFT)
                direction = PDIR_LEFT;
            else if (key == key_set->RIGHT)
                direction = PDIR_RIGHT;
            else if (key == key_set->STOP)
                direction = PDIR_STOP;

            else if (key == key_set->FIRE)
            {
                bullet_manager->shoot(cur_bul_name, {y, x}, direction);
            }

            // Walk
            float x_temp = x, y_temp = y;

            if (direction == PDIR_UP)
                y_temp -= speed;
            else if (direction == PDIR_DOWN)
                y_temp += speed;
            else if (direction == PDIR_LEFT)
                x_temp -= speed;
            else if (direction == PDIR_RIGHT)
                x_temp += speed;

            if (map->get_map()[(int)y_temp][(int)x_temp] == 1)
            {
                // is wall cannot go
                beep();
            }
            else
            {
                y = y_temp;
                x = x_temp;
            }

            clock->wait(1);
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