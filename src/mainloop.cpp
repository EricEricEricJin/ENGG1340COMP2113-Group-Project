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

float distance(std::pair<int, int> p1, std::pair<int, int> p2);

void mainloop()
{
    // Load map files, initialize UI, and inquire setting
    auto maps = game::load_all_maps();
    game::UI ui;
    int map_id, difficulty;
    ui.homepage(maps, map_id, difficulty);

    // Initialize Map with selected map
    game::Map map(LINES, COLS);
    map.load(game::map_id_to_fp(map_id));

    // bullet list
    std::vector<game::Bullet> bullet_list;

    // initialize player
    game::Player player(bullet_list, map);

    std::vector<game::Zombie> zombie_list;

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