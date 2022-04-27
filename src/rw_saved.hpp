#ifndef _RW_SAVED_H
#define _RW_SAVED_H
#include <iostream>
#include <tuple>
#include <fstream>
#include <json.hpp>

#include "zombie.h"
#include "bullet.h"
#include "player.h"
#include "clock.h"

namespace game
{

    std::tuple<bool, zombieManager *, bulletManager *, Player *, Map *, Clock*> load_saved_game(std::string path)
    {
        nlohmann::json json_data;
        std::ifstream file_stream(path, std::fstream::in);
        // Open resource file
        try
        {
            file_stream >> json_data;
            file_stream.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            file_stream.close();
            return std::make_tuple(false, nullptr, nullptr, nullptr, nullptr, nullptr);
        }

        auto player_j = json_data["player"];

        zombieManager* zombie_manager;
        bulletManager* bullet_manager;
        Player* player;
        Map* map;
        Clock* clock;

        zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);
        bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
        // Do later


    }
    void save_saved_game(zombieManager *zombie_manager, bulletManager *bullet_manager, Player *player, Map *map, std::string path)
    {

    }
}
#endif