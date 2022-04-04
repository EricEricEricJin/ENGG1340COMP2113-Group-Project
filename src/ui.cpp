#include "ui.h"

namespace game
{
    UI::UI()
    {
        // init screen
        initscr();
        cbreak();
        timeout(0);
        noecho();
    }

    bool UI::homepage(std::vector<std::pair<std::string **, std::pair<int, int>>> maps, int &map_id, int &player_num, int &difficulty)
    {
        // <LINES, COLS>
        int _map_id = 0;
        int _player_num = -1;
        int _difficulty = -1;

        int key;
        // ask user to select map, player number, and difficulty
        /*
            Select Map: use 1, 2, 3, ... to select
            Small Map is displayed in real time
            Use -> and <- keys to shift among map / pnum / diffi
            Use Enter key to confirm
        */

    SELECT_MAP: // jump out when press -> or <- or Enter
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
            if (key == KEY_RIGHT)
                goto SET_PLAYER_NUM;
            else if (key == KEY_LEFT)
                goto SET_DIFFICULTY;
            else if (key == KEY_ENTER)
                goto HP_RETURN;

            usleep(50000); // 50ms
        }

    SET_PLAYER_NUM:
        for (;;)
        {
            move(LINES / 2 - 1, COLS / 2 - 1);
            addstr("Player number: ");
            
        }

        SET_DIFFICULTY:

        HP_RETURN:
            map_id = _map_id;
        player_num = _player_num;
        difficulty = _difficulty;
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

    // void UI::refresh()
    // {
    //     clear();
    //     _draw_walls();
    //     _draw_zombies();
    //     _draw_bullets();
    //     _draw_players();
    //     refresh();
    // }

    void UI::start_game(std::vector<Player> &_player_list, std::vector<Zombie> &_zombie_list, Map &_map)
    {
    }

    void UI::get_op(bool &running, int &p1_dir, int &p1_shooting, int &p1_weapon, int &p2_dir, int &p2_shooting, int &p2_weapon)
    {
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
