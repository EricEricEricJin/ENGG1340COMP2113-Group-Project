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
#include "setting.hpp"

float distance(std::pair<int, int> p1, std::pair<int, int> p2);

void mainloop()
{
    // game::Setting *setting = new game::Setting();
    // if (setting->load())
    // {
    //     // config file exists
    //     std::string map_dir = setting->query<std::string>("map_dir");
    //     std::string bullet_dir = setting->query<std::string>("bullet_dir");
    // }
    // else
    // {
    //     // default values
    // }

    game::Clock *clock = new game::Clock;
    clock->set_freq(10);
    clock->reset();
    clock->start();

    game::Map *map = new game::Map("resource/map/");

    game::bulletManager *bullet_manager = new game::bulletManager();
    game::zombieManager *zombie_manager = new game::zombieManager();
    game::Player *player = new game::Player();
    game::UI *ui = new game::UI();

    player->init(bullet_manager, map, clock, ui->get_key_ptr());
    std::cout << "Player initialized" << std::endl;
    bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
    std::cout << "Bullet initialized" << std::endl;
    zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);
    std::cout << "Zombie initialized" << std::endl;

    bullet_manager->load_resource("resource/");


    ui->init(player, zombie_manager->get_zombie_list(), bullet_manager->get_bullet_list(), map, clock);

    std::string homepage_ret_string;
    int homepage_ret_kind;
    ui->homepage(&homepage_ret_string, &homepage_ret_kind);

    if (homepage_ret_kind == game::HOMEPAGE_NEWG)
        std::cout << "map load " << map->load(homepage_ret_string) << std::endl;

    std::cout << "HA" << std::endl;

    bullet_manager->run();
    zombie_manager->run();
    player->run();
    ui->start_game();

    while (player->get_hp() > 0)
    {
        clock->wait(1);
    }
    ui->stop_game();

    delete zombie_manager;
    delete player;
    delete bullet_manager;
    delete map;
    delete ui;

    endwin();
}