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

    game::bulletManager *bullet_manager = new game::bulletManager();
    game::zombieManager *zombie_manager = new game::zombieManager();
    game::Player *player = new game::Player();

    player->init(bullet_manager, map, clock);
    std::cout << "Player initialized" << std::endl;
    bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
    std::cout << "Bullet initialized" << std::endl;
    zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);
    std::cout << "Zombie initialized" << std::endl;

    bullet_manager->load_resource("resource/");
    std::cout << "map load " << map->load("0.json") << std::endl;

    bullet_manager->run();
    zombie_manager->run();

    game::UI* ui = new game::UI();
    ui->init(player, zombie_manager->get_zombie_list(), bullet_manager->get_bullet_list(), map, clock);

    player->run();
    ui->start_game();
    zombie_manager->add(ZOMBIETYPE_ODNR, map->zb_get_rand_ent_yx());

    for (int i = 0; i < 200; i++)
    {
        clock->wait(1);
    }
    ui->stop_game();
    
    endwin();
}