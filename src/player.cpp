#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player()
    {
    }

    void Player::init(bulletManager *_bullet_manager, Map *_map, Clock *_clock, int *_key_ptr)
    {
        bullet_manager = _bullet_manager;
        map = _map;
        clock = _clock;
        key_ptr = _key_ptr;

        key_set = new playerKeySet;

        speed = 0.8;

        direction = PDIR_RIGHT;
        hp = 100;
    }

    void Player::load_saved(std::pair<int, int> _yx, int _hp, int _dir, std::string _cur_bul_name)
    {
        y = _yx.first;
        x = _yx.second;
        hp = _hp;
        direction = _dir;
        cur_bul_name = _cur_bul_name;
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
                key = *key_ptr;
            else
            {
                // std::cout << "Player key value: ";
                // std::cin >> key;
            }
            // mvprintw(LINES - 2, 1, "%d", direction);
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
                if (!cur_bul_name.empty())
                    bullet_manager->shoot(cur_bul_name, {y, x}, direction);
            }

            else if ('1' <= key && key <= '9')
            {
                // shift weapon
                if (int bul_id = key - '1'; bul_id >= 0 && bul_id < bullet_manager->get_names().size())
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

            if (map->get_bit(round(y_temp), round(x_temp)))
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

    std::string Player::get_cur_bul_name() { return cur_bul_name; }

    Player::~Player()
    {
        stop();
        delete key_set;
    }
}