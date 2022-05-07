#include "player.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    Player::Player() { thread_obj = nullptr; }

    void Player::init(bulletManager *bullet_manager, Map *map, Clock *clock, int *key_ptr)
    {
        this->bullet_manager = bullet_manager;
        this->map = map;
        this->clock = clock;
        this->key_ptr = key_ptr;

        key_set = new playerKeySet;
        speed = 0.8;
        reset();
    }

    void Player::set_variables(std::pair<int, int> yx, int hp, int dir, std::string cur_bul_name, int score)
    {
        this->y = yx.first;
        this->x = yx.second;
        this->hp = hp;
        this->direction = dir;
        this->cur_bul_name = cur_bul_name;
        set_score(score);
    }

    void Player::get_variables(std::pair<int, int> &yx, int &hp, int &dir, std::string &cur_bul_name, int &score)
    {
        yx = {this->y, this->x};
        hp = this->hp;
        dir = this->direction;
        cur_bul_name = this->cur_bul_name;
        score = this->score;
    }

    void Player::configure(playerKeySet new_keyset) { *key_set = new_keyset; }

    float Player::get_hp() { return hp; }

    bool Player::set_hp(float new_hp)
    {
        hp = new_hp;
        if (hp > 0)
            return true;
        else
            return false;
    }

    std::pair<float, float> Player::get_yx() { return {y, x}; }

    void Player::reset()
    {
        y = map->player_get_init_yx().first;
        x = map->player_get_init_yx().second;
        direction = PDIR_STOP;
        last_nonstop_dir = PDIR_RIGHT;
        hp = 100;
        set_score(0);
    }

    chtype Player::get_char()
    {
        if (last_nonstop_dir == PDIR_UP)
            return ACS_UARROW;
        if (last_nonstop_dir == PDIR_DOWN)
            return ACS_DARROW;
        if (last_nonstop_dir == PDIR_LEFT)
            return ACS_LARROW;
        return ACS_RARROW;
    }

    void Player::set_score(int new_score)
    {
        score_lock.lock();
        score = new_score;
        score_lock.unlock();
    }

    int Player::get_score() { return score; }

    void Player::run(bool debug)
    {
        paused = false;

        running = true;
        _debug = debug;
        thread_obj = new std::thread([=]
                                     { _player_thread_loop(); });
    }

    void Player::_player_thread_loop()
    {
        int key = 0;
        clock_tick_t waiting_ticks;
        while (running)
        {
            waiting_ticks = clock->get_ticks();
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
                        bullet_manager->shoot(cur_bul_name, {y, x}, last_nonstop_dir, clock->get_ticks());
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

                if (direction != PDIR_STOP)
                    last_nonstop_dir = direction;

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
                set_score(get_score() + 1);
            }
            clock->wait_until(waiting_ticks + 1);
        }
    }

    void Player::stop()
    {
        running = false;
        if (thread_obj != nullptr)
        {
            thread_obj->join();
            delete thread_obj;
            thread_obj = nullptr;
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