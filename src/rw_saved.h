#ifndef _RW_SAVED_H
#define _RW_SAVED_H
#include <iostream>
#include <tuple>
#include <fstream>
#include <experimental/filesystem>
#include <json.hpp>

#include "zombie.h"
#include "bullet.h"
#include "player.h"
#include "clock.h"
#include "map.h"

namespace game
{
    class rwSaved
    {
    private:
        std::string saving_dir;

        nlohmann::json* json_data;

        zombieManager* zombie_manager;
        bulletManager* bullet_manager;
        Player* player;
        Map* map;
        Clock* clock;
    public:
        // rwSaved();
        void init(zombieManager* zombie_manager, bulletManager* bullet_manager, Player* player, Map* map, Clock* clock, std::string saving_dir);
        std::vector<std::string> get_all_savings();
        bool read_set(std::string name);
        bool get_write(std::string name);
        bool delete_saving(std::string name);
        // ~rwSaved();
    };
}

#endif