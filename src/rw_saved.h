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
#include "map.h"

namespace game
{
    class rwSaved
    {
    private:
        nlohmann::json* json_data;

        zombieManager* zombie_manager;
        bulletManager* bullet_manager;
        Player* player;
        Map* map;
        Clock* clock;
    public:
        // rwSaved();
        void init(zombieManager* zombie_manager, bulletManager* bullet_manager, Player* player, Map* map, Clock* clock);
        bool read_set(std::string path);
        bool get_write(std::string path);
        // ~rwSaved();
    };
}

#endif