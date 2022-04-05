#include "ui.h"

namespace game
{
    UI::UI()
    {
        // init screen
        initscr();
        nocbreak();
        timeout(0);
        noecho();
    }

    bool UI::homepage(std::vector<std::pair<std::string **, std::pair<int, int>>> maps, int &map_id, int &difficulty)
    {
        // This page: block input
        cbreak();
        timeout(-1);

        // <LINES, COLS>
        int _map_id = 0;
        // int _player_num = 0;
        int _difficulty = 0;

        int key = 0;
        // ask user to select map, player number, and difficulty

        for (;;)
        {
            // Select Map
            for (;;)
            {
                // show map
                int map_height = maps[_map_id].second.first;
                int map_width = maps[_map_id].second.second;

                if ((LINES < map_height) || (COLS < map_width))
                    return false; // Window to small. Cannot display

                int x_offset = (LINES - map_width) / 2;
                int y_offset = (COLS - map_height) / 2;

                // display minimap
                for (int y = 0; y < map_height; y++)
                {
                    for (int x = 0; x < map_width; x++)
                    {
                        move(y + y_offset, x + x_offset);
                        addstr(maps[_map_id].first[y][x].c_str());
                    }
                }

                // display number
                move(0, COLS - 2);
                addstr(std::to_string(_map_id).c_str());

                key = getch();

                if (key == 'j') // next map
                {
                    _map_id++;
                    if (_map_id >= maps.size())
                        _map_id = 0;
                }
                else if (key == 'k') // prev map
                {
                    _map_id--;
                    if (_map_id < 0)
                        _map_id = maps.size() - 1;
                }
                else if (key == '\n')
                    break;
            }

            // Select player number
            // for (;;)
            // {
            //     move(LINES / 2 - 1, COLS / 2 - 10);
            //     addstr("Number of players: ");

            //     char pn_str[10];
            //     move(LINES / 2 - 1, COLS / 2 + 10);
            //     if (_player_num != 0)
            //         addstr(std::to_string(_player_num).c_str());
            //     getstr(pn_str);
            //     std::istringstream(pn_str) >> _player_num;
            //     if (_player_num >= 1 && _player_num <= MAX_PLAYER_NUM)
            //         break;
            // }

            // Select difficulty
            for (;;)
            {
                move(LINES / 2 - 1, COLS / 2 - 10);
                addstr("Level of difficulty: ");

                char diff_level[10];
                move(LINES / 2 - 1, COLS / 2 + 10);
                getstr(diff_level);
                std::istringstream(diff_level) >> _difficulty;
                if (_difficulty >= 1 && _difficulty <= MAX_DIFF_LEVEL)
                {
                    break;
                }
                // prompt warning
                        }

            // View selection:
            move(LINES / 2 - 5, COLS / 2 - 10);
            addstr("Map No.:");
            move(LINES / 2 - 5, COLS / 2 + 5);
            addstr(std::to_string(_map_id + 1).c_str());

            // move(LINES / 2 - 3, COLS / 2 - 10);
            // addstr("# of Players:");
            // move(LINES / 2 - 3, COLS / 2 + 5);
            // addstr(std::to_string(_player_num).c_str());

            move(LINES / 2 - 1, COLS / 2 - 10);
            addstr("Difficulty:");
            move(LINES / 2 - 1, COLS / 2 + 5);
            addstr(std::to_string(_difficulty).c_str());

            move(LINES / 2 - 4, COLS / 2 - 10);
            addstr("Press Enter to confirm setting");
            move(LINES / 2 - 5, COLS / 2 - 10);
            addstr("Press any other keys to re select");

            key = getch();
            if (key == '\n')
                break;
        }

        map_id = _map_id;
        // player_num = _player_num;
        difficulty = _difficulty;
        nocbreak();
        timeout(0);
        return true;
    }

    void UI::start_game(std::vector<Player> &_player_list, std::vector<Zombie> &_zombie_list, Map &_map)
    {
        // set dependencies
        player_list = &_player_list;
        zombie_list = &_zombie_list;
        map = &_map;

        running = true;
        std::thread(_game_thread_loop); // launch game loop
        // stop when running become false
    }

    void UI::_game_thread_loop()
    {
        // NOTE: DO NOT MODIFY ANY VALUES IN the three lists
        while (running)
        {
            // update_op: update user op
            // menu: prompt menu
            // refresh: move zombies, bullets, and players
        }
    }

    void UI::start_game(std::vector<Player> &_player_list, std::vector<Zombie> &_zombie_list, Map &_map)
    {
    }

    void UI::get_op(bool &running, int &p1_dir, int &p1_shooting, int &p1_weapon, int &p2_dir, int &p2_shooting, int &p2_weapon)
    {
        int key = getch();
    }

    void UI::_draw_walls()
    {
        for (int r = 0; r < map->lines(); r++)
        {
            for (int c = 0; c < map->columns(); c++)
            {
                if (map->get_char(c, r) != "")
                {
                    move(r, c);
                    addstr(map->get_char(r, c).c_str());
                }
            }
        }
    }

    void UI::_draw_players()
    {
        for (auto &player : *player_list)
        {
            move(player.get_xy()[1], player.get_xy()[0]);
            addstr(player.get_char().c_str());
        }
    }

    void UI::_draw_zombies()
    {
        for (auto &zombie : *zombie_list)
        {
            move(zombie.get_xy()[1], zombie.get_xy()[0]);
            addstr(zombie.get_char().c_str());
        }
    }

    void UI::_draw_bullets()
    {
        for (auto &bullet : *bullet_list)
        {
            move(bullet.get_xy()[1], bullet.get_xy()[0]);
            addstr(bullet.get_char().c_str());
        }
    }

    void UI::_show_score(std::vector<int> scores)
    {
        // Show on the top
        for (int i = 0; i < scores.size(); i++)
        {
            int x = i * (COLS / scores.size());
            move(0, x);
            addstr(("player " + std::to_string(i + 1) + ": " + std::to_string(scores[i])).c_str());
        }
    }

}
