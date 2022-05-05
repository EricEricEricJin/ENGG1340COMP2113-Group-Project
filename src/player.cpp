#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player()
    {
        thread_obj = nullptr;
    }

    void Player::init(bulletManager *bullet_manager, Map *map, Clock *clock, int *key_ptr)
    {
        this->bullet_manager = bullet_manager;
        this->map = map;
        this->clock = clock;
        this->key_ptr = key_ptr;

        key_set = new playerKeySet;

        speed = 0.8;

        direction = PDIR_RIGHT;
        hp = 100;
    }

    void Player::set_variables(std::pair<int, int> yx, int hp, int dir, std::string cur_bul_name)
    {
        this->y = yx.first;
        this->x = yx.second;
        this->hp = hp;
        this->direction = dir;
        this->cur_bul_name = cur_bul_name;
    }

    void Player::get_variables(std::pair<int, int> &yx, int &hp, int &dir, std::string &cur_bul_name)
    {
        yx = {this->y, this->x};
        hp = this->hp;
        dir = this->direction;
        cur_bul_name = this->cur_bul_name;
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
        paused = false;
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
            if (!paused)
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
                        bullet_manager->shoot(cur_bul_name, {y, x}, direction, clock->get_ticks());
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
            }
            clock->wait(1);
        }
    }

    void Player::stop()
    {
        running = false;
        if (thread_obj != nullptr)
        {
            thread_obj->join();
            delete thread_obj;
        }
    }

    void Player::pause() { paused = true; }
    void Player::resume() { paused = false; }

    std::string Player::get_cur_bul_name() { return cur_bul_name; }

    Player::~Player()
    {
        stop();
        delete key_set;
    }
}