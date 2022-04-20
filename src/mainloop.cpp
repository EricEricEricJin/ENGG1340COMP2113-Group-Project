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

void mainloop()
{
    game::Clock *clock = new game::Clock;
    clock->set_freq(10);
    clock->reset();
    clock->start();

    game::Map *map = new game::Map("resource/map/");

    game::bulletManager *bullet_manager;
    game::zombieManager *zombie_manager;
    game::UI *ui;
    game::Player *player;

    bullet_manager = new game::bulletManager(map, zombie_manager->get_zombie_list(), player, clock);
    std::cout << "Bullet initialized" << std::endl;

    zombie_manager = new game::zombieManager(bullet_manager->get_bullet_list(), map, player, clock);
    std::cout << "Zombie initialized" << std::endl;

    ui = new game::UI(player, zombie_manager->get_zombie_list(), bullet_manager->get_bullet_list(), map, clock);
    std::cout << "UI initialized" << std::endl;

    player = new game::Player(bullet_manager, map, clock);
    std::cout << "Player initialized" << std::endl;

    bullet_manager->load_resource("resource/");

    std::string map_name;
    int difficulty;
    ui->init();
    ui->homepage(&map_name, &difficulty);
    map->load(map_name);
    ui->start_game();

    bullet_manager->run();
    player->run();
    zombie_manager->run();

    int exit_code;
    while (true)
    {
        // if ui quit then break
        if (ui->status() == UI_PAUSE)
        {
            continue;
        }
        else if (ui->status() == UI_QUIT)
        {
            exit_code = 1;
            break;
        }
        clock->wait(1);
    }
}