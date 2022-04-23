#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player()
    {
    }

    void Player::init(bulletManager *_bullet_manager, Map *_map, Clock *_clock)
    {
        bullet_manager = _bullet_manager;
        map = _map;
        clock = _clock;

        // default keysey
        key_set = new playerKeySet{'w', 's', 'a', 'd', 'e', ' '};
        speed = 0.5;

        hp = 100;
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

    void Player::run(bool debug)
    {
        if (x < 0 || y < 0)
        {
            y = map->player_get_init_yx().first;
            x = map->player_get_init_yx().second;
        }

        running = true;
        _debug = debug;
        thread_obj = new std::thread([=]
                                     { _player_thread_loop(); });
    }

    void Player::_player_thread_loop()
    {
        int key = 0;
        while (running)
        {
            if (!_debug)
                key = getch();
            else
            {
                // std::cout << "Player key value: ";
                // std::cin >> key;
            }
            mvprintw(LINES - 2, 1, "%d", direction);
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

            else if ('1' <= key && key <= '9')
            {
                // shift weapon
                int bul_id = key - '1';
                cur_bul_name = bullet_manager->get_names()[bul_id];
            }

            else if (key == 96) // ` key
            {
                // put wall
                map->add(round(y), round(x), 100);
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

            if (map->get_bit((int)round(y_temp), (int)round(x_temp)))
            {
                // is wall or out-of-map, cannot go
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

    Player::~Player()
    {
        delete key_set;
    }
}