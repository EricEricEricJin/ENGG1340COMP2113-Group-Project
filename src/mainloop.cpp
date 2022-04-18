/*

    DO:
    - Initialize UI
    - UI hp
    - Initialize Map
    - Initialize player
    - player.run
    - // enter mainloop
    - Add zombies
    - zombie.run
    - Player add Bullets
    - zombie add bullets
    - for each bullet
        if triggered
            calculate damage


*/

#include "mainloop.h"
#include <queue>
#include <vector>

#include "ui.h"
#include "map.h"
#include "bullet.h"
#include "player.h"
#include "zombie.h"
#include "clock.h"

float distance(std::pair<int, int> p1, std::pair<int, int> p2);

std::vector<game::Zombie *> zombie_list;

void mainloop()
{
    // Initialize map
    game::Map *map = new game::Map();
    map->load("../resource/map/");

    // Initialize bullet
    game::bulletManager *bullet_manager = new game::bulletManager();
    bullet_manager->load_resource("../resource/");
    
    game::zombieManager* zombie_manager = new game::zombieManager();

    // Initialize UI
    game::UI* ui = new game::UI();

    int difficulty;
    std::string map_name;
    ui->homepage(&map_name, &difficulty);


    // Initialize Map with selected map
    map->load(map_name);

    // bullet manager

    // initialize player
    game::Player *player = new game::Player();

    std::vector<game::Zombie *> *zombie_list = new std::vector<game::Zombie *>;

    ui.start_game(player, zombie_list, map);

    player.run();

    int exit_code;
    while (true)
    {
        // if ui quit then break
        if (ui.get_status() == UI_PAUSE)
        {
            continue;
        }
        else if (ui.get_status() == UI_QUIT)
        {
            exit_code = 1;
            break;
        }

        // monitor bullets
        for (auto &bullet : bullet_list)
        {
            if (bullet.triggered())
            {
                // player
                if (player.set_hp(player.get_hp() - bullet.damage(distance(player.get_xy(), bullet.get_xy()))) == false)
                {
                    // player dead
                    exit_code = 0;
                    goto EXIT_MAINLOOP;
                }

                // zombie
                for (auto &zombie : zombie_list)
                {
                    if (zombie.set_hp(bullet.damage(distance(zombie.get_xy(), bullet.get_xy()))) == false)
                    {
                        // zombie dead
                        delete &zombie;
                        zombie_list.pop_back();
                    }
                }

                // walls
            }
        }

        // add zombies
        for (int i = 0; i < 100 - zombie_list.size(); i++)
        {
            zombie_list.push_back(game::Zombie(bullet_list, map, player));
            zombie_list.back().run();
        }
    }

EXIT_MAINLOOP:
    ui.exit_game();
}