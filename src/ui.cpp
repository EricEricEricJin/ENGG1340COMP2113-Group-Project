#include "ui.h"
#include "player.h"
#include "map.h"
#include "zombie.h"
#include "bullet.h"

namespace game
{
    UI::UI(Player *_player, std::list<Zombie *> *_zombie_list, std::list<Bullet *> *_bullet_list, Map *_map, Clock *_clock)
    {
        player = _player;
        zombie_list = _zombie_list;
        bullet_list = _bullet_list;
        map = _map;
        clock = _clock;
    }

    void UI::init()
    {
        // init screen
        initscr();
        nocbreak();
        timeout(0);
        noecho();
    }

    bool UI::homepage(std::string *map_name, int *difficulty)
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
        auto map_names = map->names_of_maps();

        if (map_names.size() == 0)
            return false;

        for (;;)
        {
            // Select Map
            for (;;)
            {
                // show map
                auto mapvec = map->minimap(map_names[_map_id]);
                int map_height = mapvec.size();
                int map_width = mapvec[0].size();

                if ((LINES < map_height) || (COLS < map_width))
                    return false; // Window to small. Cannot display

                int x_offset = (COLS - map_width) / 2;
                int y_offset = (LINES - map_height) / 2;

                // display minimap
                clear();
                refresh();
                for (int y = 0; y < map_height; y++)
                {
                    for (int x = 0; x < map_width; x++)
                    {
                        move(y + y_offset, x + x_offset);
                        addstr(mapvec[y][x].c_str());
                    }
                }

                // display number
                move(0, COLS - 2);
                addstr(std::to_string(_map_id).c_str());

                key = getch();

                if (key == 'j') // next map
                {
                    _map_id++;
                    if (_map_id >= map_names.size())
                        _map_id = 0;
                }
                else if (key == 'k') // prev map
                {
                    _map_id--;
                    if (_map_id < 0)
                        _map_id = map_names.size() - 1;
                }
                else if (key == '\n')
                    break;
            }

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

        *map_name = map_names[_map_id];
        *difficulty = _difficulty;
        nocbreak();
        timeout(0);
        return true;
    }

    void UI::start_game()
    {
        // set dependencies
        status_val = 1;
        thread_obj = new std::thread([=]
                                     { _game_thread_loop(); }); // launch game loop
        // stop when running become false
    }

    void UI::stop_game()
    {
        status_val = 0;
        thread_obj->join();
        delete thread_obj;
    }

    void UI::_game_thread_loop()
    {
        // NOTE: DO NOT MODIFY ANY VALUES IN the three lists
        while (status_val)
        {
            // update_op: update user op
            // menu: prompt menu
            // refresh: move zombies, bullets, and players
            if (getch() == MENU_KEY)
            {
                _game_menu();
            }
            _draw_walls();
            _draw_players();
            _draw_zombies();
            _draw_bullets();
            _show_info();
            clock->wait(1); // 20Hz
        }
    }

    int UI::status()
    {
        return status_val;
    }

    void UI::_game_menu()
    {
        status_val = STATUS_MENU;

        cbreak();
        timeout(-1);

        int key;
        for (;;)
        {
            key = getch();
            if (key == 27)
                _bottom_mode();
        }
        status_val = STATUS_RUNNING;
    }

    void UI::_bottom_mode() {}

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
        move(player->get_yx().first, player->get_yx().second);
        addstr(player->get_char().c_str());
    }

    void UI::_draw_zombies()
    {
        for (auto &zombie : *zombie_list)
        {
            move(zombie->get_yx().first, zombie->get_yx().second);
            addstr(zombie->get_char().c_str());
        }
    }

    void UI::_draw_bullets()
    {
        for (auto &bullet : *bullet_list)
        {
            move(bullet->get_yx().first, bullet->get_yx().second);
            addstr(bullet->get_char().c_str());
        }
    }

    void UI::_show_info()
    {
        // Show on the top
    }

    UI::~UI()
    {
        endwin();
    }

}
