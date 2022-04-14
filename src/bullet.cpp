#include "bullet.h"
#include "map.h"
#include "player.h"
#include "zombie.h"

#include <filesystem>
#include <fstream>

#include "tinyexpr.h"

namespace game
{
    bulletManager::bulletManager()
    {
    }

    int bulletManager::load_resource(std::string resource_root)
    {
        using namespace std;

        filesystem::path str(resource_root + "/bullet/");
        if (filesystem::exists(str))
            return 0;
        filesystem::directory_entry entry(str);
        if (entry.status().type() != filesystem::file_type::directory)
            return 0;
        filesystem::directory_iterator list(str);

        for (auto &_name : list)
        {
            auto name = _name.path().filename().string();
            // open and parse file
            ifstream f(resource_root + "/bullet/" + name, fstream::in);
            if (f.fail())
                continue;
            nlohmann::json json_data;
            try
            {
                f >> json_data;
                f.close();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
                f.close();
                return 0;
            }

            auto type = json_data["type"].get<string>();

            bul_type_dict[type] = new bullet_t;
            bul_type_dict[type]->speed = json_data["speed"].get<float>();
            bul_type_dict[type]->chr = json_data["char"].get<string>();
            bul_type_dict[type]->trig_mode = json_data["trig_mode"].get<int>();
            bul_type_dict[type]->damage_dist = json_data["damage_dist"].get<int>();

            for (auto &i : json_data["trig_obj"])
                bul_type_dict[type]->trig_obj.push_back(i.get<string>());

            te_variable vars[] = {{"x", &temp_distance}};
            bul_type_dict[type]->damage_func = te_compile(json_data["damage_expr"].get<string>().c_str(), vars, 1, nullptr);
        }
        return 1;
    }

    void bulletManager::run(Map *_map, std::vector<Zombie *> *_zombie_list, Player *_player)
    {
        map = _map;
        zombie_list = _zombie_list;
        player = _player;
    }

    void bulletManager::_thread_loop()
    {
        // DO NOT DO ANY INITIALIZE HERE
        // DO IT IN FUNCTION `run`
        while (running)
        {
            int b_it = -1;
            for (auto &bullet : bullet_list)
            {
                b_it++;
                bullet_t *bullet_type = bul_type_dict[bullet->type];

                if (bullet_type->trig_mode == TRIG_CONTACT)
                {
                    if (std::count(bullet_type->trig_obj.begin(), bullet_type->trig_obj.end(), "zombie"))
                        for (auto &zombie : *zombie_list)
                            if (pair_distance(zombie->get_xy(), bullet->xy) > bullet_type->trig_c)
                                goto TRIGGERED;
                }
                else if (bullet_type->trig_mode == TRIG_TIMER && *timer - bullet->shoot_time >= bullet_type->trig_c)
                    goto TRIGGERED;
                continue;

            TRIGGERED:
                // Find all zombies in distance
                for (auto &zombie : *zombie_list)
                    if (pair_distance(zombie->get_xy(), bullet->xy) < bullet_type->damage_dist)
                    {
                        temp_distance = pair_distance(zombie->get_xy(), bullet->xy);
                        float damage = te_eval(bullet_type->damage_func);
                        zombie->set_hp(zombie->get_hp() - damage);
                    }

                // player
                if (pair_distance(player->get_xy(), bullet->xy) <= bullet_type->damage_dist)
                {
                    temp_distance = pair_distance(player->get_xy(), bullet->xy);
                    float damage = te_eval(bullet_type->damage_func);
                    player->set_hp(player->get_hp() - damage);
                }

                // destroy bullet
                delete bullet;
                bullet_list.erase(bullet_list.begin() + b_it);
            }
        }
    }

    void bulletManager::shoot(std::string name, int x, int y, int dir)
    {
        Bullet *b = new Bullet{name, *timer, {x, y}, dir};
        bullet_list.push_back(b);
    }

    void bulletManager::pause()
    {
        running = false;
        thread_obj->join(); // block wait until thread complete
        delete thread_obj;
    }

    void bulletManager::resume()
    {
        running = true;
        thread_obj = new std::thread([=]
                                     { _thread_loop(); });
        // cannot call non-static member function directly
    }
}