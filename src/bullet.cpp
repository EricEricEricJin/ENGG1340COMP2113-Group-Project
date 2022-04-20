#include "bullet.h"
#include "map.h"
#include "player.h"
#include "zombie.h"

#include <filesystem>
#include <fstream>

#include "tinyexpr.h"

float pair_distance(std::pair<float, float> p1, std::pair<float, float> p2)
{
    return ((p1.first - p2.first) * (p1.first - p2.first) +
            (p1.second - p2.second) * (p1.second - p2.second));
}

namespace game
{
    Bullet::Bullet(std::string _type, clock_tick_t _shoot_time, std::pair<float, float> _yx, int _dir)
    {
        type = _type;
        shoot_time = _shoot_time;
        yx = _yx;
        dir = _dir;
    }

    std::pair<float, float> Bullet::get_yx() { return yx; }

    std::string Bullet::get_type() { return type; }

    std::string Bullet::get_char() { return character; }

    clock_tick_t Bullet::get_shoot_time() { return shoot_time; }

    bulletManager::bulletManager(Map *_map, std::list<Zombie *> *_zombie_list, Player *_player, Clock *_clock)
    {
        map = _map;
        zombie_list = _zombie_list;
        player = _player;
        clock = _clock;
    }

    int bulletManager::load_resource(std::string resource_root)
    {
        using namespace std;

        filesystem::path str(resource_root + "bullet"); // Cannot have "/" at end
        cout << "fp: " << str << endl;
        if (!filesystem::exists(str))
            return 0;
        cout << "exist" << endl;
        filesystem::directory_entry entry(str);
        if (entry.status().type() != filesystem::file_type::directory)
            return 0;
        filesystem::directory_iterator list(str);

        for (auto &_name : list)
        {
            auto name = _name.path().filename().string();
            cout << "name: " << name << endl;
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

            bul_type_dict[type] = new bulletType;
            bul_type_dict[type]->speed = json_data["speed"].get<float>();
            bul_type_dict[type]->chr = json_data["char"].get<string>();
            bul_type_dict[type]->trig_mode = json_data["trig_mode"].get<int>();
            bul_type_dict[type]->damage_dist = json_data["damage_dist"].get<float>();

            bul_type_dict[type]->trig_c = json_data["trig_c"].get<float>();

            for (auto &i : json_data["trig_obj"])
                bul_type_dict[type]->trig_obj.push_back(i.get<string>());

            te_variable vars[] = {{"x", &temp_distance}};
            bul_type_dict[type]->damage_func = te_compile(json_data["damage_expr"].get<string>().c_str(), vars, 1, nullptr);
        }
        return 1;
    }

    void bulletManager::print()
    {
        using namespace std;
        for (auto &bul_type : bul_type_dict)
        {
            cout << bul_type.first << ": " << endl;
            cout << "character " << bul_type.second->chr << endl;
            cout << "speed " << bul_type.second->speed << endl;
            cout << "trig_mode " << bul_type.second->trig_mode << endl;
            cout << "damage_dist " << bul_type.second->damage_dist << endl;
            cout << "trig_c" << bul_type.second->trig_c << endl;

            cout << "trig_obj ";
            for (auto &obj : bul_type.second->trig_obj)
                cout << obj << " ";
            cout << endl;
        }
    }

    std::list<Bullet *> *bulletManager::get_bullet_list()
    {
        return bullet_list;
    }

    void bulletManager::run()
    {
        running = true;
        thread_obj = new std::thread([=]
                                     { _thread_loop(); });
    }

    void bulletManager::_thread_loop()
    {
        // DO NOT DO ANY INITIALIZE HERE
        // DO IT IN FUNCTION `run`
        std::list<Bullet *>::iterator bul_iter;
        bool triggered;

        while (running)
        {
            triggered = false;

            for (auto bul_iter = bullet_list->begin(); bul_iter != bullet_list->end();)
            {
                auto bullet = *bul_iter;
                bulletType *bullet_type = bul_type_dict[bullet->get_type()];

                if (bullet_type->trig_mode == TRIG_CONTACT)
                {
                    if (std::count(bullet_type->trig_obj.begin(), bullet_type->trig_obj.end(), "zombie"))
                        for (auto &zombie : *zombie_list)
                            if (pair_distance(zombie->get_yx(), bullet->get_yx()) > bullet_type->trig_c)
                            {
                                triggered = true;
                                break;
                            }
                }
                else if (bullet_type->trig_mode == TRIG_TIMER && clock->get_ticks() - bullet->get_shoot_time() >= bullet_type->trig_c)
                    triggered = true;

                if (triggered)
                {
                    // Find all zombies in distance
                    for (auto &zombie : *zombie_list)
                        if (pair_distance(zombie->get_yx(), bullet->get_yx()) < bullet_type->damage_dist)
                        {
                            temp_distance = pair_distance(zombie->get_yx(), bullet->get_yx());
                            float damage = te_eval(bullet_type->damage_func);
                            zombie->set_hp(zombie->get_hp() - damage);
                        }

                    // player
                    if (pair_distance(player->get_yx(), bullet->get_yx()) <= bullet_type->damage_dist)
                    {
                        temp_distance = pair_distance(player->get_yx(), bullet->get_yx());
                        float damage = te_eval(bullet_type->damage_func);
                        player->set_hp(player->get_hp() - damage);
                    }

                    // destroy bullet
                    delete bullet;
                    bullet_list->erase(bul_iter);
                }
                else
                    bul_iter++;
            }
            clock->wait(1);
        }
    }

    void bulletManager::shoot(std::string name, std::pair<int, int> yx, int dir)
    {
        Bullet *b = new Bullet{name, clock->get_ticks(), yx, dir};
        bullet_list->push_back(b);
    }

    void bulletManager::stop()
    {
        running = false;
        thread_obj->join(); // block wait until thread complete
        delete thread_obj;
    }
    bulletManager::~bulletManager()
    {
    }
}