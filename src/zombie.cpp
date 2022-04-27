#include "zombie.h"
#include "bullet.h"
#include "player.h"
#include "map.h"

#include "solve_maze.h"

namespace game
{
    // Zombie object:
    Zombie::Zombie(std::pair<int, int> yx, int _type, int _dmg_delay)
    {
        y = yx.first;
        x = yx.second;
        type = _type;

        speed = 0.2;
        damage = 10;
        hp = 100;

        last_dmg_time = -9999;
        dmg_delay = _dmg_delay; // Should be read from config file. Implement later
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
    }

    void zombieManager::init(std::list<Bullet *> *_bullet_list, Map *_map, Player *_player, Clock *_clock)
    {
        bullet_list = _bullet_list;
        map = _map;
        player = _player;
        clock = _clock;
    }

    void zombieManager::add(int type, std::pair<int, int> yx)
    {
        if (type == ZOMBIETYPE_ODNR)
        {
            zombie_list->push_back(new Zombie(yx, ZOMBIETYPE_ODNR, 2));
        }
        else if (type == ZOMBIETYPE_KING)
        {
            // TODO later
        }
    }
    std::list<Zombie *> *zombieManager::get_zombie_list() { return zombie_list; }

    int zombieManager::get_num() { return zombie_list->size(); }

    void zombieManager::run()
    {
        running = true;
        thread_obj = new std::thread([=]
                                     { _thread_loop(); });
    }

    void zombieManager::stop()
    {
        running = false;
        thread_obj->join();
        delete thread_obj;
    }

    void zombieManager::_thread_loop()
    {
        // Add zombies

        char **zombie_map = new char *[map->lines()];
        for (int i = 0; i < map->lines(); i++)
            zombie_map[i] = new char[map->columns()];

        const int add_zombie_delay = 5;
        int add_zombie_time = -1;

        while (running)
        {
            if (get_num() < 4 && clock->get_ticks() > add_zombie_time + add_zombie_delay)
            {
                add(ZOMBIETYPE_ODNR, map->zb_get_rand_ent_yx());
                add_zombie_time = clock->get_ticks();
            }

            for (auto zombie_it = zombie_list->begin(); zombie_it != zombie_list->end();)
            {
                // If zombie dead, then remove
                if ((*zombie_it)->get_hp() <= 0)
                {
                    zombie_list->erase(zombie_it++);
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
                        zombie_map[(int)round((*z)->get_yx().first)][(int)round((*z)->get_yx().second)] = 1;
                }

                int dir = solve_maze(zombie_map, map->lines(), map->columns(), {round((*zombie_it)->get_yx().first), round((*zombie_it)->get_yx().second)}, player->get_yx(), 1, 0);
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
                        if ((*zombie_it)->last_dmg_time + (*zombie_it)->dmg_delay >= clock->get_ticks())
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

            clock->wait(1);
        }
        delete zombie_map;
    }

    zombieManager::~zombieManager()
    {
        for (auto &zombie : *zombie_list)
            delete zombie;
        delete zombie_list;
    }

}