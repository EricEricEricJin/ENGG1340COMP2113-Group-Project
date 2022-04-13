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

    int bulletManager::load_resource()
    {
        using namespace std;

        filesystem::path str(BUL_PATH);
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
            ifstream f(BUL_PATH + name, fstream::in);
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

            char_dict[type] = json_data["char"].get<string>();
            speed_dict[type] = json_data["speed"].get<float>();

            // @TODO
            auto a = new[](float distance, string obj)
            {

                te_eval("");
            };
            trigger_dict[type] = &a;
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
            // ...
        }
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
        thread_obj = new std::thread(_thread_loop);
    }

}