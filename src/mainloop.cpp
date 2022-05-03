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
#include <filesystem>

#include "ui.h"
#include "map.h"
#include "bullet.h"
#include "player.h"
#include "zombie.h"
#include "clock.h"
#include "setting.h"

float distance(std::pair<int, int> p1, std::pair<int, int> p2);

void mainloop()
{
    std::cout << "Loading configuration..." << std::endl;
    std::string boxheadrc_path = std::string(getenv("HOME")) + "/.boxhead/init.bh";
    std::string resource_path = std::string(getenv("HOME")) + "/.boxhead/resource/";
    int clock_frequency = 12;

    game::playerKeySet player_keyset{'w', 's', 'a', 'd', 'e', ' '};

    game::uiKeySet ui_keyset{'k', 'j', '\n'};

    if (getenv("BOXHEADRC") != nullptr)
        boxheadrc_path = getenv("BOXHEADRC");

    game::Setting *setting = new game::Setting();
    if (setting->load(boxheadrc_path))
    {
        if (setting->get_resource_path() != "")
            resource_path = setting->get_resource_path();

        if (setting->get_clock_frequency() > 0)
            clock_frequency = setting->get_clock_frequency();

        auto setting_keyset = setting->get_keyset();
        if (setting_keyset.size() == 6)
        {
            player_keyset.UP = setting_keyset[0];
            player_keyset.DOWN = setting_keyset[1];
            player_keyset.LEFT = setting_keyset[2];
            player_keyset.RIGHT = setting_keyset[3];
            player_keyset.STOP = setting_keyset[4];
            player_keyset.FIRE = setting_keyset[5];
        }
    }

    // print config
    std::cout << "Configuration load." << std::endl;
    std::cout << "    Clock frequency: " << clock_frequency << std::endl
              << "    Player key set: " << std::endl
              << "            UP " << player_keyset.UP << std::endl
              << "          DOWN " << player_keyset.DOWN << std::endl
              << "          LEFT " << player_keyset.LEFT << std::endl
              << "         RIGHT " << player_keyset.RIGHT << std::endl
              << "          STOP " << player_keyset.STOP << std::endl
              << "          FIRE " << player_keyset.FIRE << std::endl;

    std::cout << "Initializing map..." << std::endl;
    game::Map *map = new game::Map(resource_path + "map/");
    if (map->names_of_maps().empty())
    {
        std::cout << "Error: Map resource not found!" << std::endl;
        delete setting;
        delete map;
        return;
    }
    std::cout << "Map initialized, " << map->names_of_maps().size() << " maps found." << std::endl;

    game::Clock *clock = new game::Clock;
    clock->set_freq(clock_frequency);
    clock->reset();
    clock->start();

    game::bulletManager *bullet_manager = new game::bulletManager();
    game::zombieManager *zombie_manager = new game::zombieManager();
    game::Player *player = new game::Player();
    game::UI *ui = new game::UI();

    player->init(bullet_manager, map, clock, ui->get_key_ptr());
    bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
    zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);

    std::cout << "Initializing bullet..." << std::endl;
    bullet_manager->load_resource(resource_path + "bullet/");
    if (bullet_manager->get_names().empty())
    {
        std::cout << "Error: Bullet resource not found!" << std::endl;
        delete map;
        delete clock;
        delete bullet_manager;
        delete zombie_manager;
        delete player;
        delete ui;
    }
    std::cout << "Bullet initialized, " << bullet_manager->get_names().size() << " bullets found." << std::endl;

    player->configure(player_keyset);

    ui->init(player, zombie_manager->get_zombie_list(), bullet_manager->get_bullet_list(), map, clock);
    ui->configure(ui_keyset, game::UTHEME_LIGHT | game::UTHEME_BORD);

    std::string homepage_ret_string;
    int homepage_ret_kind;
    ui->homepage(&homepage_ret_string, &homepage_ret_kind);

    if (homepage_ret_kind == game::HOMEPAGE_NEWG)
        std::cout << "map load " << map->load(homepage_ret_string) << std::endl;

    bullet_manager->run();
    zombie_manager->run();
    player->run();
    ui->start_game();

    while (player->get_hp() > 0 && ui->get_status() != game::USTATUS_EXIT)
    {
        clock->wait(1);
        // std::cout << "clock tick " << clock->get_ticks() << std::endl;
    }
    ui->stop_game();

    delete ui;
    delete zombie_manager;
    delete player;
    delete bullet_manager;
    delete map;
    delete clock;
    endwin();
}