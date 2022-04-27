/*
    Homepage:
    |-- Change setting
        |-- clock_frequency
        |-- resource_dir
    |-- New game
        |-- Map
        |-- Difficulty
    |-- Load from saving
        |-- Select saving
*/

#include "ui.h"
#include "player.h"
#include "zombie.h"
#include "map.h"
#include "bullet.h"

namespace game
{
    UI::UI() {}

    void UI::init(Player *_player, std::list<Zombie *> *_zombie_list, std::list<Bullet *> *_bullet_list, Map *_map, Clock *_clock)
    {
        player = _player;
        zombie_list = _zombie_list;
        bullet_list = _bullet_list;
        map = _map;
        clock = _clock;

        initscr();
        noecho();
        nocbreak();
        timeout(0);

        // box(stdscr, 0, 0);
        // refresh();
    }

    int *UI::get_key_ptr() { return &key; }

    bool UI::homepage(std::string *ret_string, int *ret_kind)
    {
        // setting
        cbreak();
        timeout(-1);

        WIN_OFFSET_Y = (LINES - WIN_HEIGHT) / 2;
        WIN_OFFSET_X = (COLS - WIN_WIDTH) / 2;
        WINDOW *home_win = newwin(24, 80, WIN_OFFSET_Y, WIN_OFFSET_X);

        std::array<std::string, 5> items = {"New game", "Load saving", "Setting", "Edit map", "Exit"};
        int current_item = 0;

        while (true)
        {
            wclear(home_win);
            box(home_win, 0, 0);

            // display menu
            for (int i = 0; i < items.size(); i++)
            {
                if (current_item == i)
                    wattron(home_win, A_REVERSE);
                mvwaddstr(home_win, WIN_HEIGHT / items.size() * i + 1, WIN_WIDTH / 2 + 1, items[i].c_str());

                if (current_item == i)
                    wattroff(home_win, A_REVERSE);
            }
            wrefresh(home_win);

            int _key = wgetch(home_win);
            if (_key == 'j')
                current_item++;
            else if (_key == 'k')
                current_item--;
            else if (_key == '\n')
            {
                wclear(home_win);
                wrefresh(home_win);
                if (current_item == HOMEPAGE_NEWG)
                {
                    bool _ret = _new_game_page(ret_string);
                    if (_ret)
                    {
                        *ret_kind = HOMEPAGE_NEWG;
                        break;
                    }
                }
                else if (current_item == HOMEPAGE_LOAD)
                {
                    *ret_string = _load_saving_page();
                    *ret_kind = HOMEPAGE_LOAD;
                }
                else if (current_item == HOMEPAGE_SETT)
                {
                    _setting_page();
                    *ret_kind = HOMEPAGE_SETT;
                }
                else if (current_item == HOMEPAGE_EDIT)
                {
                    _edit_map_page();
                    *ret_kind = HOMEPAGE_EDIT;
                }
                else if (current_item == HOMEPAGE_EXIT)
                {
                    *ret_kind = HOMEPAGE_EXIT;
                    break;
                };
            }

            if (current_item < 0)
                current_item = items.size();
            else if (current_item >= items.size())
                current_item = 0;
        }

        delwin(home_win);
        nocbreak();
        timeout(0);
        return false;
    }

    bool UI::_new_game_page(std::string *map_name)
    {
        // select map
        WINDOW *list_win = newwin(WIN_HEIGHT, 12, WIN_OFFSET_Y, WIN_OFFSET_X);
        box(list_win, 0, 0);
        WINDOW *preview_win = nullptr;

        auto map_names = map->names_of_maps();
        int map_idx = 0;

        bool ret;

        while (true)
        {
            // display name list
            for (int i = 0; i < map_names.size(); i++)
            {
                if (map_idx == i)
                    wattron(list_win, A_REVERSE);
                mvwaddstr(list_win, i + 1, 1, map_names[i].c_str());
                if (map_idx == i)
                    wattroff(list_win, A_REVERSE);
            }
            wrefresh(list_win);

            // draw minimap
            auto minimap = map->minimap(map_names[map_idx]);
            if (preview_win != nullptr)
                delwin(preview_win);
            preview_win = newwin(minimap.size() + 2, minimap[0].length() + 2, WIN_OFFSET_Y, WIN_OFFSET_X + 12);
            box(preview_win, 0, 0);

            int x, y = 1;
            for (auto &line : minimap)
            {
                x = 1;
                for (auto &chr : line)
                {
                    mvwaddch(preview_win, y, x, chr);
                    x++;
                }
                y++;
            }
            wrefresh(preview_win);

            int _key = wgetch(list_win);
            if (_key == 'j')
                map_idx++;
            else if (_key == 'k')
                map_idx--;
            else if (_key == '\n')
            {
                ret = true;
                break;
            }
            else if (_key == 'q')
            {
                ret = false;
                break;
            }

            if (map_idx < 0)
                map_idx = map_names.size() - 1;
            else if (map_idx >= map_names.size())
                map_idx = 0;
        }

        delwin(preview_win);
        delwin(list_win);
        *map_name = map_names[map_idx];
        return ret;
    }

    std::string UI::_load_saving_page() {}

    void UI::_setting_page() {}
    void UI::_edit_map_page() {}

    void UI::start_game()
    {
        int y_0 = (LINES - map->lines() - 5) / 2;
        int width = (map->columns() + 2);
        int x_0 = (COLS - width) / 2;

        status_win = newwin(3, width, y_0, x_0);

        game_win = newwin(map->lines() + 2, width, y_0 + 3, x_0);

        // start thread...
        status_val = 1;
        thread_obj = new std::thread([=]
                                     { _game_thread_loop(); });
    }

    void UI::_game_thread_loop()
    {
        // draw and refresh screen
        while (status_val == 1)
        {

            wclear(game_win);
            box(game_win, 0, 0);

            // map
            for (int i = 0; i < map->lines(); i++)
            {
                for (int j = 0; j < map->columns(); j++)
                {
                    mvwaddstr(game_win, i + 1, j + 1, map->get_char(i, j).c_str());
                }
            }

            // player
            mvwaddstr(game_win, (int)round(player->get_yx().first + 1), (int)round(player->get_yx().second + 1), player->get_char().c_str());

            // zombie
            for (auto &zombie : *zombie_list)
            {
                mvwaddstr(game_win, (int)round(zombie->get_yx().first) + 1, (int)round(zombie->get_yx().second) + 1, zombie->get_char().c_str());
            }

            // bullet
            for (auto &bullet : *bullet_list)
            {
                mvwaddstr(game_win, (int)round(bullet->get_yx().first) + 1, (int)round(bullet->get_yx().second) + 1, bullet->get_char().c_str());
            }

            wrefresh(game_win);
            // key = wgetch(game_win);
            key = getch();

            // HP and bullet
            wclear(status_win);
            box(status_win, 0, 0);

            wattron(status_win, A_REVERSE);
            mvwprintw(status_win, 1, 1, "HP        ");
            mvwprintw(status_win, 1, 21, "WEAPON    ");
            wattroff(status_win, A_REVERSE);
            mvwprintw(status_win, 1, 11, "%d", (int)(player->get_hp()));
            mvwprintw(status_win, 1, 31, player->get_cur_bul_name().c_str());
            wrefresh(status_win);
            clock->wait(1);
        }
    }

    void UI::_game_menu()
    {
        // WINDOW* menu_win = newwin()
    }

    void UI::stop_game()
    {
        status_val = 0;
        thread_obj->join();
        delwin(game_win);
        delwin(status_win);
        delete thread_obj;
    }

    int UI::get_status() { return status_val; }

    UI::~UI() {}

}