#include "bullet.h"
#include "map.h"
#include "player.h"
#include "zombie.h"

#include <filesystem>
#include <fstream>

#include "tinyexpr.h"

float pair_distance(std::pair<float, float> p1, std::pair<float, float> p2)
{
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) +
                (p1.second - p2.second) * (p1.second - p2.second));
}

namespace game
{
    Bullet::Bullet(std::string type, clock_tick_t shoot_time, std::pair<float, float> yx, int dir, std::string character)
    {
        this->type = type;
        this->shoot_time = shoot_time;
        this->yx = yx;
        this->dir = dir;
        this->character = character;
    }

    std::pair<float, float> Bullet::get_yx() { return yx; }

    int Bullet::get_dir() { return dir; }

    std::string Bullet::get_type() { return type; }

    std::string Bullet::get_char() { return character; }

    clock_tick_t Bullet::get_shoot_time() { return shoot_time; }

    void Bullet::move(std::pair<float, float> new_yx) { yx = new_yx; }

    bulletManager::bulletManager()
    {
        bullet_list = new std::list<Bullet *>;
        thread_obj = nullptr;
    }

    void bulletManager::init(Map *map, std::list<Zombie *> *zombie_list, Player *player, Clock *clock)
    {
        this->map = map;
        this->zombie_list = zombie_list;
        this->player = player;
        this->clock = clock;
    }

    int bulletManager::load_resource(std::string resource_path)
    {
        using namespace std;

        filesystem::path str(resource_path); // Must have "/" at end
        if (!filesystem::exists(str))
            return 0;
        filesystem::directory_entry entry(str);
        if (entry.status().type() != filesystem::file_type::directory)
            return 0;
        filesystem::directory_iterator list(str);

        for (auto &_name : list)
        {
            auto name = _name.path().filename().string();
            // open and parse file
            ifstream f(resource_path + name, fstream::in);

            std::string type;

            if (f.fail())
            {
                f.close();
                return 0;
            }

            nlohmann::json json_data;
            try
            {
                f >> json_data;
                f.close();

                type = json_data["type"].get<string>();

                bul_type_dict[type] = new bulletType;
                bul_type_dict[type]->speed = json_data["speed"].get<float>();
                bul_type_dict[type]->chr = json_data["char"].get<string>();
                bul_type_dict[type]->trig_mode = json_data["trig_mode"].get<int>();
                bul_type_dict[type]->damage_dist = json_data["damage_dist"].get<float>();

                bul_type_dict[type]->trig_c = json_data["trig_c"].get<float>();

                if (bul_type_dict[type]->trig_mode == TRIG_CONTACT)
                    for (auto &i : json_data["trig_obj"])
                        bul_type_dict[type]->trig_obj.push_back(i.get<string>());

                for (auto &i : json_data["damage_obj"])
                    bul_type_dict[type]->damage_obj.push_back(i.get<std::string>());

                te_variable vars[] = {{"x", &temp_distance}};
                bul_type_dict[type]->damage_func = te_compile(json_data["damage_expr"].get<string>().c_str(), vars, 1, nullptr);
            }
            catch (const std::exception &e)
            {
                // std::cerr << e.what() << '\n';
                f.close();
                if (!type.empty())
                {
                    delete bul_type_dict[type];
                    bul_type_dict.erase(type);
                }
                return 0;
            }
        }
        return 1;
    }

    void bulletManager::set_variables(std::vector<std::string> types, std::vector<int> shoot_times, std::vector<std::pair<int, int>> yxs, std::vector<int> dirs)
    {
        for (int i = 0; i < types.size(); i++)
        {
            auto bullet = new Bullet(types[i], shoot_times[i], yxs[i], dirs[i], bul_type_dict[types[i]]->chr);
            bullet_list->push_back(bullet);
        }
    }

    void bulletManager::get_variables(std::vector<std::string> &types, std::vector<int> &shoot_times, std::vector<std::pair<int, int>> &yxs, std::vector<int> &dirs)
    {
        for (auto &bullet : *bullet_list)
        {
            types.push_back(bullet->get_type());
            shoot_times.push_back(bullet->get_shoot_time());
            yxs.push_back(bullet->get_yx());
            dirs.push_back(bullet->get_dir());
        }
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

    void bulletManager::reset()
    {
        bullet_list->clear();
    }

    void bulletManager::run()
    {
        paused = false;
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

        clock_tick_t waiting_ticks;

        while (running)
        {
            waiting_ticks = clock->get_ticks();
            if (!paused)
            {
                for (auto bul_iter = bullet_list->begin(); bul_iter != bullet_list->end();)
                {
                    triggered = false;
                    auto bullet = *bul_iter;
                    bulletType *bullet_type = bul_type_dict[bullet->get_type()];

                    // Contact trigger
                    if (bullet_type->trig_mode == TRIG_CONTACT)
                    {
                        // contact with zombie
                        if (std::count(bullet_type->trig_obj.begin(), bullet_type->trig_obj.end(), "zombie"))
                        {
                            for (auto &zombie : *zombie_list)
                            {
                                if (pair_distance(zombie->get_yx(), bullet->get_yx()) < bullet_type->trig_c)
                                {
                                    triggered = true;
                                    break;
                                }
                            }
                        }

                        // contact with wall
                        if (std::count(bullet_type->trig_obj.begin(), bullet_type->trig_obj.end(), "wall"))
                        {
                            if (map->get_bit(round(bullet->get_yx().first), round(bullet->get_yx().second)) == 1)
                            {
                                // Is wall
                                triggered = true;
                            }
                        }
                    }
                    // Timer trigger
                    else if (bullet_type->trig_mode == TRIG_TIMER && clock->get_ticks() - bullet->get_shoot_time() >= bullet_type->trig_c)
                        triggered = true;

                    // // Out-of-map
                    // if (map->get_bit(bullet->get_yx().first, bullet->get_yx().second))
                    //     triggered = true;

                    if (triggered)
                    {
                        // std::cout << "TRIGGERED" << std::endl;
                        // Find all zombies in distance
                        if (std::count(bullet_type->damage_obj.begin(), bullet_type->damage_obj.end(), "zombie"))
                        {
                            for (auto &zombie : *zombie_list)
                            {
                                if (pair_distance(zombie->get_yx(), bullet->get_yx()) < bullet_type->damage_dist)
                                {
                                    temp_distance = pair_distance(zombie->get_yx(), bullet->get_yx());
                                    float damage = te_eval(bullet_type->damage_func);
                                    // std::cout << "Distance: " << temp_distance << " Damage: " << damage << std::endl;
                                    zombie->set_hp(zombie->get_hp() - damage);
                                }
                            }
                        }

                        // player
                        if (std::count(bullet_type->damage_obj.begin(), bullet_type->damage_obj.end(), "player"))
                        {
                            if (pair_distance(player->get_yx(), bullet->get_yx()) <= bullet_type->damage_dist)
                            {
                                temp_distance = pair_distance(player->get_yx(), bullet->get_yx());
                                float damage = te_eval(bullet_type->damage_func);
                                player->set_hp(player->get_hp() - damage);
                            }
                        }

                        // wall
                        if (std::count(bullet_type->damage_obj.begin(), bullet_type->damage_obj.end(), "wall"))
                        {
                            for (int i = 0; i < map->lines(); i++)
                            {
                                for (int j = 0; j < map->columns(); j++)
                                {
                                    if (map->get_bit(i, j) && pair_distance({i, j}, bullet->get_yx()) <= bullet_type->damage_dist)
                                    {
                                        temp_distance = pair_distance({i, j}, bullet->get_yx());
                                        map->damage(i, j, te_eval(bullet_type->damage_func));
                                    }
                                }
                            }
                        }
                        // destroy bullet
                        delete bullet;
                        bullet_list->erase(bul_iter++);
                        /*
                            list iterator: cannot have something like {erase(it); it++;}
                        */
                    }
                    else
                    {
                        // bullet move
                        if (bullet->get_dir() == BDIR_UP)
                            bullet->move({bullet->get_yx().first - 1, bullet->get_yx().second});
                        else if (bullet->get_dir() == BDIR_DOWN)
                            bullet->move({bullet->get_yx().first + 1, bullet->get_yx().second});
                        else if (bullet->get_dir() == BDIR_LEFT)
                            bullet->move({bullet->get_yx().first, bullet->get_yx().second - 1});
                        else if (bullet->get_dir() == BDIR_RIGHT)
                            bullet->move({bullet->get_yx().first, bullet->get_yx().second + 1});

                        bul_iter++;
                    }
                }
            }
            clock->wait_until(waiting_ticks + 1);
        }
    }

    void bulletManager::shoot(std::string name, std::pair<float, float> yx, int dir, clock_tick_t shoot_time)
    {
        Bullet *b = new Bullet{name, shoot_time, yx, dir, bul_type_dict[name]->chr};
        bullet_list->push_back(b);
    }

    void bulletManager::stop()
    {
        running = false;
        if (thread_obj != nullptr)
        {
            thread_obj->join(); // block wait until thread complete
            delete thread_obj;
            thread_obj = nullptr;
        }
    }

    void bulletManager::pause()
    {
        paused = true;
    }

    void bulletManager::resume() { paused = false; }

    std::vector<std::string> bulletManager::get_names()
    {
        std::vector<std::string> ret;
        for (auto &bt : bul_type_dict)
            ret.push_back(bt.first);
        return ret;
    }

    bulletManager::~bulletManager()
    {
        stop();
        reset();
        delete bullet_list;
    }
}