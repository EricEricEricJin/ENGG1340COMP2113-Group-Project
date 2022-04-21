#include "map.h"
#include "bullet.h"
#include "player.h"
#include "zombie.h"
#include "clock.h"

int main()
{
    game::Clock *clock = new game::Clock;
    clock->set_freq(2);
    clock->reset();
    clock->start();

    game::Map *map = new game::Map("../resource/map/");

    game::bulletManager *bullet_manager;
    game::zombieManager *zombie_manager;
    game::Player *player;

    std::list<game::Zombie *> *zombie_list;
    std::list<game::Bullet *> *bullet_list;

    std::cout << "HA" << std::endl;

    bullet_manager = new game::bulletManager(map, zombie_list, player, clock);
    std::cout << "Bullet initialized" << std::endl;

    zombie_manager = new game::zombieManager(bullet_list, map, player, clock);
    std::cout << "Zombie initialized" << std::endl;

    zombie_list = zombie_manager->get_zombie_list();
    bullet_list = bullet_manager->get_bullet_list();

    player = new game::Player(bullet_manager, map, clock);
    std::cout << "Player initialized" << std::endl;

    bullet_manager->load_resource("../resource/");
    map->load("0.json");

    bullet_manager->run();
    zombie_manager->run();
    player->run(true);

    int i;

    while (true)
    {
        clock->wait(1);

        // print player
        std::cout << ">>> Player:" << std::endl;
        std::cout << "HP " << player->get_hp() << std::endl;
        std::cout << "Y " << player->get_yx().first << " X " << player->get_yx().second << std::endl;
        std::cout << std::endl;

        // print zombies
        std::cout << ">>> Zombies:" << std::endl;
        i = 0;
        for (auto &zombie : *zombie_list)
        {
            std::cout << "no " << i;
            std::cout << " HP " << zombie->get_hp() << " Y " << zombie->get_yx().first << " X " << zombie->get_yx().second << std::endl;
            i++;
        }
        std::cout << std::endl;

        // print bullets
        std::cout << ">>> Bullets:" << std::endl;
        i = 0;
        for (auto &bullet : *bullet_list)
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