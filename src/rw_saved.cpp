#include "rw_saved.h"

namespace game
{
    void rwSaved::init(zombieManager *zombie_manager, bulletManager *bullet_manager, Player *player, Map *map, Clock *clock, std::string saving_dir)
    {
        this->zombie_manager = zombie_manager;
        this->bullet_manager = bullet_manager;
        this->player = player;
        this->map = map;
        this->clock = clock;
        this->saving_dir = saving_dir;

        json_data = nullptr;
    }

    std::vector<std::string> rwSaved::get_all_savings()
    {
        using namespace std;
        using namespace std::experimental::filesystem;

        vector<string> ret;
        path str(saving_dir);
        if (!exists(str))
            return ret; // fail
        directory_entry entry(str);
        if (entry.status().type() != file_type::directory)
            return ret;
        directory_iterator list(str);
        for (auto &i : list)
            ret.push_back(i.path().filename());
        return ret;
        ;
    }

    bool rwSaved::read_set(std::string name)
    {
        auto path = saving_dir + name;
        // open file
        std::ifstream f(path);
        if (f.fail())
        {
            f.close();
            return false;
        }

        // clear json
        if (json_data != nullptr)
        {
            delete json_data;
            json_data = nullptr;
        };
        json_data = new nlohmann::json();

        try
        {
            f >> (*json_data);
            f.close();
        }
        catch (const std::exception &e)
        {
            delete json_data;
            json_data = nullptr;
            f.close();
            return false;
        }

        try
        {
            // Player
            std::pair<int, int> player_yx = {
                (*json_data)["player"]["y"].get<int>(),
                (*json_data)["player"]["x"].get<int>()};
            int player_hp = (*json_data)["player"]["hp"].get<int>();
            int player_dir = (*json_data)["player"]["dir"].get<int>();
            std::string player_bul_name = (*json_data)["player"]["cur_bul_name"].get<std::string>();
            int player_score = (*json_data)["player"]["score"].get<int>();

            // Zombies
            std::vector<int> zombie_types;
            std::vector<std::pair<int, int>> zombie_yxs;
            std::vector<int> zombie_hps;
            int zombie_tar_num;

            for (auto &zombie : (*json_data)["zombies"])
            {
                zombie_types.push_back(zombie["type"].get<int>());
                zombie_yxs.push_back({zombie["y"].get<int>(), zombie["x"].get<int>()});
                zombie_hps.push_back(zombie["hp"].get<int>());
            }
            zombie_tar_num = (*json_data)["zombie_tar_num"];

            // Bullets
            std::vector<std::string> bullet_types;
            std::vector<int> bullet_shoot_times;
            std::vector<std::pair<int, int>> bullet_yxs;
            std::vector<int> bullet_dirs;

            for (auto &bullet : (*json_data)["bullets"])
            {
                bullet_types.push_back(bullet["type"].get<std::string>());
                bullet_shoot_times.push_back(bullet["shoot_time"].get<int>());
                bullet_yxs.push_back({bullet["y"].get<int>(),
                                      bullet["x"].get<int>()});
                bullet_dirs.push_back(bullet["dir"].get<int>());
            }

            // Map
            std::string map_name = (*json_data)["map"]["name"].get<std::string>();
            std::vector<std::vector<int>> map_wall_duras;
            for (auto &line : (*json_data)["map"]["wall_duras"])
            {
                std::vector<int> line_v = {};
                for (auto &v : line)
                    line_v.push_back(v.get<int>());
                map_wall_duras.push_back(line_v);
            }

            // Clock
            int clock_ticks = (*json_data)["clock_ticks"].get<int>();

            zombie_manager->set_variables(zombie_types, zombie_yxs, zombie_hps, zombie_tar_num);
            bullet_manager->set_variables(bullet_types, bullet_shoot_times, bullet_yxs, bullet_dirs);
            player->set_variables(player_yx, player_hp, player_dir, player_bul_name, player_score);
            map->set_variables(map_name, map_wall_duras);
            clock->set_ticks(clock_ticks);

            delete json_data;
            json_data = nullptr;
            return true;
        }
        catch (const std::exception &e)
        {
            delete json_data;
            json_data = nullptr;
            return false;
        }
    }

    bool rwSaved::get_write(std::string name)
    {
        auto path = saving_dir + name;

        std::ofstream f(path);
        if (f.fail())
        {
            f.close();
            return false;
        }

        std::pair<int, int> player_yx;
        int player_hp;
        int player_dir;
        std::string player_bul_name;
        int player_score;

        std::vector<int> zombie_types;
        std::vector<std::pair<int, int>> zombie_yxs;
        std::vector<int> zombie_hps;
        int zombie_tar_num;

        std::vector<std::string> bullet_types;
        std::vector<int> bullet_shoot_times;
        std::vector<std::pair<int, int>> bullet_yxs;
        std::vector<int> bullet_dirs;

        std::string map_name;
        std::vector<std::vector<int>> map_wall_duras;

        player->get_variables(player_yx, player_hp, player_dir, player_bul_name, player_score);
        zombie_manager->get_variables(zombie_types, zombie_yxs, zombie_hps, zombie_tar_num);
        bullet_manager->get_variables(bullet_types, bullet_shoot_times, bullet_yxs, bullet_dirs);
        map->get_variables(map_name, map_wall_duras);
        int clock_ticks = clock->get_ticks();

        if (json_data != nullptr)
        {
            delete json_data;
            json_data = nullptr;
        }

        json_data = new nlohmann::json();

        // player
        (*json_data)["player"]["y"] = player_yx.first;
        (*json_data)["player"]["x"] = player_yx.second;
        (*json_data)["player"]["hp"] = player_hp;
        (*json_data)["player"]["dir"] = player_dir;
        (*json_data)["player"]["cur_bul_name"] = player_bul_name;
        (*json_data)["player"]["score"] = player_score;

        // zombies
        for (int i = 0; i < zombie_types.size(); i++)
        {
            (*json_data)["zombies"].push_back(
                std::map<std::string, int>{
                    {"type", zombie_types[i]},
                    {"y", zombie_yxs[i].first},
                    {"x", zombie_yxs[i].second},
                    {"hp", zombie_hps[i]}});
        }
        (*json_data)["zombie_tar_num"] = zombie_tar_num;

        // bullets
        for (int i = 0; i < bullet_types.size(); i++)
        {
            (*json_data)["bullets"].push_back({std::pair<std::string, int>{"shoot_time", bullet_shoot_times[i]},
                                               std::pair<std::string, int>{"y", bullet_yxs[i].first},
                                               std::pair<std::string, int>{"x", bullet_yxs[i].second},
                                               std::pair<std::string, int>{"dir", bullet_dirs[i]},
                                               std::pair<std::string, std::string>{"type", bullet_types[i]}});
        }

        // map
        (*json_data)["map"]["name"] = map_name;
        (*json_data)["map"]["wall_duras"] = map_wall_duras;

        // clock
        (*json_data)["clock_ticks"] = clock_ticks;

        // save to file
        bool ret;
        try
        {
            f << (*json_data);
            ret = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            ret = false;
        }

        f.close();
        delete json_data;
        json_data = nullptr;
        return ret;
    }

    bool rwSaved::delete_saving(std::string name)
    {
        return std::experimental::filesystem::remove(saving_dir + name);
    }
}