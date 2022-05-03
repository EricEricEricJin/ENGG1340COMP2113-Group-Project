#include "map.h"
#include "bullet.h"
#include "player.h"
#include "zombie.h"
#include "clock.h"
#include "ui.h"

int main()
{
    game::Clock *clock = new game::Clock;
    clock->set_freq(5);
    clock->reset();
    clock->start();

    game::Map *map = new game::Map("../resource/map/");

    game::bulletManager *bullet_manager = new game::bulletManager();
    game::zombieManager *zombie_manager = new game::zombieManager();
    game::Player *player = new game::Player();

    player->init(bullet_manager, map, clock);
    std::cout << "Player initialized" << std::endl;
    bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
    std::cout << "Bullet initialized" << std::endl;
    zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);
    std::cout << "Zombie initialized" << std::endl;

    bullet_manager->load_resource("../resource/");
    std::cout << "map load " << map->load("0.json") << std::endl;

    bullet_manager->run();
    zombie_manager->run();
    player->run(true);

    zombie_manager->add(ZOMBIETYPE_ODNR, {10, 1});
    bullet_manager->shoot("9mm", player->get_yx(), game::BDIR_LEFT);

    // print map
    for (int i = 0; i < map->lines(); i++)
    {
        for (int j = 0; j < map->columns(); j++)
        {
            if (map->get_map()[i][j] == 1)
                std::cout << '#';
            else
                std::cout << ' ';
        }
        std::cout << std::endl;
    }
    int i;

    while (clock->get_ticks() < 100)
    {
        clock->wait(1);

        // print player
        std::cout << ">>> Player:" << player << std::endl;
        std::cout << "HP " << player->get_hp() << std::endl;
        std::cout << "Y " << player->get_yx().first << " X " << player->get_yx().second << std::endl;
        std::cout << std::endl;

        // print zombies
        std::cout << ">>> Zombies:" << std::endl;
        i = 0;
        for (auto &zombie : *(zombie_manager->get_zombie_list()))
        {
            std::cout << "no " << i;
            std::cout << " HP " << zombie->get_hp() << " Y " << zombie->get_yx().first << " X " << zombie->get_yx().second << std::endl;
            i++;
        }
        std::cout << std::endl;

        // print bullets
        std::cout << ">>> Bullets:" << std::endl;
        i = 0;
        for (auto &bullet : *(bullet_manager->get_bullet_list()))
        {
            std::cout << "no " << i;
            std::cout << " Y " << bullet->get_yx().first << " X " << bullet->get_yx().second << " Type " << bullet->get_type() << std::endl;
            i++;
        }
        std::cout << std::endl
                  << std::endl;
    }

    return 0;
}