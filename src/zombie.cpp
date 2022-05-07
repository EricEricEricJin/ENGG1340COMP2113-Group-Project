#include "zombie.h"
#include "bullet.h"
#include "player.h"
#include "map.h"

#include "solve_maze.h"

namespace game
{
    // Zombie object:
    Zombie::Zombie(std::pair<int, int> yx, int type, clock_tick_t dmg_delay)
    {
        this->y = yx.first;
        this->x = yx.second;
        this->type = type;

        speed = 0.2;
        damage = 10;
        hp = 100;

        last_dmg_time = 0;
        this->dmg_delay = dmg_delay; // Should be read from config file. Implement later
    }

    int Zombie::get_type() { return type; }

    float Zombie::get_hp() { return hp; }

    bool Zombie::set_hp(float new_hp)
    {
        hp = new_hp;
        return (hp > 0);
    }

    std::pair<float, float> Zombie::get_yx() { return {y, x}; }

    void Zombie::move(std::pair<float, float> new_yx)
    {
        y = new_yx.first;
        x = new_yx.second;
    }

    float Zombie::get_damage() { return damage; }

    float Zombie::get_speed() { return speed; }

    std::string Zombie::get_char() { return character; }

    // Zombie Manager
    zombieManager::zombieManager()
    {
        zombie_list = new std::list<Zombie *>;
        thread_obj = nullptr;
    }

    void zombieManager::init(std::list<Bullet *> *bullet_list, Map *map, Player *player, Clock *clock)
    {
        this->bullet_list = bullet_list;
        this->map = map;
        this->player = player;
        this->clock = clock;
    }

    void zombieManager::set_difficulty(int difficulty)
    {
        tar_num = difficulty;
    }

    void zombieManager::set_variables(std::vector<int> zombie_types, std::vector<std::pair<int, int>> zombie_yxs, std::vector<int> zombie_hps, int tar_num)
    {
        this->tar_num = tar_num;
        for (int i = 0; i < zombie_types.size(); i++)
        {
            auto zombie = new Zombie(zombie_yxs[i], zombie_types[i], zombie_dmg_delay);
            zombie->set_hp(zombie_hps[i]);
            zombie_list->push_back(zombie);
        }
    }

    void zombieManager::get_variables(std::vector<int> &zombie_types, std::vector<std::pair<int, int>> &zombie_yxs, std::vector<int> &zombie_hps, int &tar_num)
    {
        tar_num = this->tar_num;
        for (auto &zombie : *zombie_list)
        {
            zombie_types.push_back(zombie->get_type());
            zombie_yxs.push_back(zombie->get_yx());
            zombie_hps.push_back(zombie->get_hp());
        }
    }

    void zombieManager::add(int type, std::pair<int, int> yx)
    {
        zombie_list_lock.lock();
        if (type == ZOMBIETYPE_ODNR)
        {
            zombie_list->push_back(new Zombie(yx, ZOMBIETYPE_ODNR, 3));
        }
        else if (type == ZOMBIETYPE_KING)
        {
            // TODO later
        }
        zombie_list_lock.unlock();
    }
    std::list<Zombie *> *zombieManager::get_zombie_list() { return zombie_list; }

    int zombieManager::get_num() { return zombie_list->size(); }

    void zombieManager::reset()
    {
        zombie_list->clear();
    }

    void zombieManager::run()
    {
        paused = false;
        running = true;
        thread_obj = new std::thread([=]
                                     { _thread_loop(); });
    }

    void zombieManager::stop()
    {
        running = false;
        if (thread_obj != nullptr)
        {
            thread_obj->join();
            delete thread_obj;
            thread_obj = nullptr;
        }
    }

    void zombieManager::_thread_loop()
    {
        // Add zombies

        char **zombie_map = new char *[map->lines()];
        for (int i = 0; i < map->lines(); i++)
            zombie_map[i] = new char[map->columns()];

        const int add_zombie_delay = 5;
        int add_zombie_time = -1;

        clock_tick_t waiting_ticks;

        while (running)
        {
            waiting_ticks = clock->get_ticks();
            if (!paused)
            {
                if (get_num() < tar_num && clock->get_ticks() > add_zombie_time + add_zombie_delay)
                {
                    add(ZOMBIETYPE_ODNR, map->zb_get_rand_ent_yx());
                    add_zombie_time = clock->get_ticks();
                }

                for (auto zombie_it = zombie_list->begin(); zombie_it != zombie_list->end();)
                {
                    // If zombie dead, then remove
                    if ((*zombie_it)->get_hp() <= 0)
                    {
                        player->set_score(player->get_score() + 100);

                        zombie_list_lock.lock();
                        zombie_list->erase(zombie_it++);
                        zombie_list_lock.unlock();

                        continue;
                    }

                    // std::cout << "In zombie, player: " << player << std::endl;
                    // Move zombie

                    // TODO: copy map and modify zombie position

                    char **map_map = map->get_map();
                    for (int i = 0; i < map->lines(); i++)
                    {
                        for (int j = 0; j < map->columns(); j++)
                        {
                            zombie_map[i][j] = map_map[i][j];
                        }
                    }
                    for (auto z = zombie_list->begin(); z != zombie_list->end(); z++)
                    {
                        if (z != zombie_it)
                            zombie_map[(int)((*z)->get_yx().first)][(int)((*z)->get_yx().second)] = 1;
                    }

                    int dir = solve_maze(zombie_map, map->lines(), map->columns(), {(int)round((*zombie_it)->get_yx().first), (int)round((*zombie_it)->get_yx().second)}, {(int)round(player->get_yx().first), (int)round(player->get_yx().second)}, 1, 0);
                    if (dir == SOLMAZ_UP)
                        (*zombie_it)->move({(*zombie_it)->get_yx().first - (*zombie_it)->get_speed(), (*zombie_it)->get_yx().second});
                    else if (dir == SOLMAZ_DOWN)
                        (*zombie_it)->move({(*zombie_it)->get_yx().first + (*zombie_it)->get_speed(), (*zombie_it)->get_yx().second});
                    else if (dir == SOLMAZ_LEFT)
                        (*zombie_it)->move({(*zombie_it)->get_yx().first, (*zombie_it)->get_yx().second - (*zombie_it)->get_speed()});
                    else if (dir == SOLMAZ_RIGHT)
                        (*zombie_it)->move({(*zombie_it)->get_yx().first, (*zombie_it)->get_yx().second + (*zombie_it)->get_speed()});

                    // Ordinary zombie attack player
                    if ((*zombie_it)->get_type() == ZOMBIETYPE_ODNR)
                    {
                        if (pair_distance(player->get_yx(), (*zombie_it)->get_yx()) < 1.6) // smaller than sqrt(2)
                        {
                            // std::cout << "tick " << clock->get_ticks() << " last_t " << (*zombie_it)->last_dmg_time << " delay " << (*zombie_it)->dmg_delay << std::endl;
                            // std::cout << "True? " << ((clock->get_ticks()) > ((*zombie_it)->last_dmg_time + (*zombie_it)->dmg_delay)) << std::endl;
                            if (clock->get_ticks() > (*zombie_it)->last_dmg_time + (*zombie_it)->dmg_delay)
                            { // damage player
                                player->set_hp(player->get_hp() - (*zombie_it)->get_damage());
                                (*zombie_it)->last_dmg_time = clock->get_ticks();
                            }
                        }
                    }
                    else
                    {
                        // zombie king
                        // Shoot bullet
                        // Do later
                    }

                    zombie_it++;
                }
            }

            clock->wait_until(waiting_ticks + 1);
        }
        delete[] zombie_map;
    }

    void zombieManager::pause() { paused = true; }
    void zombieManager::resume() { paused = false; }

    zombieManager::~zombieManager()
    {
        stop();
        zombie_list->clear();
        delete zombie_list;
    }
}