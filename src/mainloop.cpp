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
#include "rw_saved.h"

float distance(std::pair<int, int> p1, std::pair<int, int> p2);

void mainloop()
{
    std::cout << "Loading configuration..." << std::endl;
    std::string boxheadrc_path = std::string(getenv("HOME")) + "/.boxhead/init.bh";
    std::string resource_path = std::string(getenv("HOME")) + "/.boxhead/resource/";
    std::string saving_path = std::string(getenv("HOME")) + "/.boxhead/saving/";
    int clock_frequency = 12;

    game::playerKeySet player_keyset{'w', 's', 'a', 'd', 'e', ' '};
    game::uiKeySet ui_keyset{'k', 'j', '\n', 'q'};
    int theme = 0;

    if (getenv("BOXHEADRC") != nullptr)
        boxheadrc_path = getenv("BOXHEADRC");

    game::Setting *setting = new game::Setting();
    if (setting->load(boxheadrc_path))
    {
        // Resource path
        if (setting->get_resource_path() != "")
            resource_path = setting->get_resource_path();

        if (setting->get_saving_path() != "")
            resource_path = setting->get_saving_path();

        // Clock frequency
        if (setting->get_clock_frequency() > 0)
            clock_frequency = setting->get_clock_frequency();

        // Player keyset
        auto setting_player_keyset = setting->get_player_keyset();
        if (setting_player_keyset.size() == 6)
        {
            if (setting_player_keyset[0])
                player_keyset.UP = setting_player_keyset[0];
            if (setting_player_keyset[1])
                player_keyset.DOWN = setting_player_keyset[1];
            if (setting_player_keyset[2])
                player_keyset.LEFT = setting_player_keyset[2];
            if (setting_player_keyset[3])
                player_keyset.RIGHT = setting_player_keyset[3];
            if (setting_player_keyset[4])
                player_keyset.STOP = setting_player_keyset[4];
            if (setting_player_keyset[5])
                player_keyset.FIRE = setting_player_keyset[5];
        }

        // UI keyset
        auto setting_ui_keyset = setting->get_ui_keyset();
        if (setting_ui_keyset.size() == 4)
        {
            if (setting_ui_keyset[0])
                ui_keyset.key_up = setting_ui_keyset[0];
            if (setting_ui_keyset[1])
                ui_keyset.key_down = setting_ui_keyset[1];
            if (setting_ui_keyset[2])
                ui_keyset.key_enter = setting_ui_keyset[2];
            if (setting_ui_keyset[3])
                ui_keyset.key_quit = setting_ui_keyset[3];
        }

        // theme
        if (auto setting_theme = setting->get_theme(); setting_theme.find("dark") != std::string::npos)
            theme |= game::UTHEME_DARK;
        else if (setting_theme.find("light") != std::string::npos)
            theme |= game::UTHEME_LIGHT;

        if (auto setting_theme = setting->get_theme(); setting_theme.find("bord") != std::string::npos)
            theme |= game::UTHEME_BORD;
        else if (setting_theme.find("thin") != std::string::npos)
            theme |= game::UTHEME_THIN;
    }

    if ((theme & 0b0011) == 0) // light/dark not set
        theme |= game::UTHEME_DARK;
    if ((theme & 0b1100) == 0) // bord / thin not set
        theme |= game::UTHEME_THIN;

    // print config
    std::cout << "Configuration load." << std::endl;
    std::cout << "    Clock frequency: " << clock_frequency << std::endl
              << "    Player key set: " << std::endl
              << "            UP " << player_keyset.UP << std::endl
              << "          DOWN " << player_keyset.DOWN << std::endl
              << "          LEFT " << player_keyset.LEFT << std::endl
              << "         RIGHT " << player_keyset.RIGHT << std::endl
              << "          STOP " << player_keyset.STOP << std::endl
              << "          FIRE " << player_keyset.FIRE << std::endl
              << "    Theme: " << theme << std::endl;

    game::bulletManager *bullet_manager = new game::bulletManager();
    game::zombieManager *zombie_manager = new game::zombieManager();
    game::Player *player = new game::Player();
    game::UI *ui = new game::UI();
    game::rwSaved *rw_saved = new game::rwSaved();
    game::Map *map = new game::Map(resource_path + "map/");
    game::Clock *clock = new game::Clock;

    std::string homepage_ret_string;
    int homepage_ret_val;
    int homepage_ret_kind;

    // Initialize UI
    if (!(ui->init(player, zombie_manager->get_zombie_list(), bullet_manager->get_bullet_list(), map, clock, rw_saved)))
    {
        std::cout << "Terminal size too small!" << std::endl;
        std::cout << "Terminal size too small!" << std::endl;
        goto MAINLOOP_END;
    }
    ui->configure(ui_keyset, theme);

    // Initialize map
    if (map->names_of_maps().empty())
    {
        ui->notice(game::UNOTICE_ERROR, "Map resource not found");
        goto MAINLOOP_END;
    }

    // Initialize clock
    clock->set_freq(clock_frequency);
    clock->reset();
    clock->start();

    // Initialize player
    player->init(bullet_manager, map, clock, ui->get_key_ptr());
    player->configure(player_keyset);

    // Initialize zombies
    zombie_manager->init(bullet_manager->get_bullet_list(), map, player, clock);

    // Initialize bullet
    bullet_manager->init(map, zombie_manager->get_zombie_list(), player, clock);
    bullet_manager->load_resource(resource_path + "bullet/");
    if (bullet_manager->get_names().empty())
    {
        ui->notice(game::UNOTICE_ERROR, "Bullet resource not found");
        goto MAINLOOP_END;
    }

    // savings
    rw_saved->init(zombie_manager, bullet_manager, player, map, clock, saving_path);

    for (;;)
    {
    UI_HOMEPAGE_START:
        ui->homepage(&homepage_ret_string, &homepage_ret_val, &homepage_ret_kind);
        zombie_manager->reset();
        bullet_manager->reset();
        player->reset();
        if (homepage_ret_kind == game::HOMEPAGE_NEWG)
        {
            if (map->load(homepage_ret_string))
            {
                if (homepage_ret_val == 0)
                    zombie_manager->set_difficulty(game::ZDIFFICULTY_HIG);
                else if (homepage_ret_val == 1)
                    zombie_manager->set_difficulty(game::ZDIFFICULTY_MED);
                else if (homepage_ret_val == 2)
                    zombie_manager->set_difficulty(game::ZDIFFICULTY_LOW);
                // break;
            }
            else
            {
                ui->notice(game::UNOTICE_ERROR, "Cannot load this map");
                goto UI_HOMEPAGE_START;
            }
        }
        else if (homepage_ret_kind == game::HOMEPAGE_LOAD)
        {
            if (rw_saved->read_set(homepage_ret_string))
            {
            }
            else
            {
                ui->notice(game::UNOTICE_ERROR, "Cannot load this saving");
                goto UI_HOMEPAGE_START;
            }
        }
        else if (homepage_ret_kind == game::HOMEPAGE_DELE)
        {
            if (rw_saved->delete_saving(homepage_ret_string))
            {
                ui->notice(game::UNOTICE_NORMAL, "Saving " + homepage_ret_string + " deleted.");
            }
            else
                ui->notice(game::UNOTICE_ERROR, "Cannot delete saving " + homepage_ret_string + ".");
        }
        else if (homepage_ret_kind == game::HOMEPAGE_EXIT)
            break;

        if (homepage_ret_kind == game::HOMEPAGE_NEWG || homepage_ret_kind == game::HOMEPAGE_LOAD)
        {
            bullet_manager->run();
            zombie_manager->run();
            player->run();
            ui->start_game();

            bool paused = false;
            while (player->get_hp() > 0 && ui->get_status() != game::USTATUS_EXIT)
            {
                clock->wait(1);
                if (bool ui_status = ui->get_status(); ui_status == game::USTATUS_PAUSE && !paused)
                {
                    paused = true;
                    player->pause();
                    bullet_manager->pause();
                    zombie_manager->pause();
                    clock->stop();
                }
                else if (ui_status == game::USTATUS_RUNNING && paused)
                {
                    paused = false;
                    player->resume();
                    bullet_manager->resume();
                    zombie_manager->resume();
                    clock->start();
                }
            }
            ui->stop_game();
            if (player->get_hp() <= 0)
                ui->notice(game::UNOTICE_NORMAL, "You died!");
        }
    }

MAINLOOP_END:
    delete ui;
    delete zombie_manager;
    delete player;
    delete bullet_manager;
    delete map;
    delete clock;
    endwin();
}