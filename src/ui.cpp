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
    }

    int *UI::get_key_ptr() { return &key; }

    bool UI::homepage(std::string *map_name, int *difficulty)
    {
        // setting
        return false;
    }

    void UI::start_game()
    {
        int y_0 = (LINES - map->lines() - 5) / 2;
        int width = (map->columns() + 2);
        int x_0 = (COLS - width) / 2;

        status_win = newwin(3, width, y_0, x_0);
        box(status_win, 0, 0);
        wrefresh(status_win);

        game_win = newwin(map->lines() + 2, width, y_0 + 3, x_0);
        box(game_win, 0, 0);
        wrefresh(game_win);

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
            key = getch();

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
            mvwaddstr(game_win, round(player->get_yx().first + 1), round(player->get_yx().second + 1), player->get_char().c_str());

            // zombie
            for (auto &zombie : *zombie_list)
            {
                mvwaddstr(game_win, zombie->get_yx().first + 1, zombie->get_yx().second + 1, zombie->get_char().c_str());
            }

            // bullet
            for (auto &bullet : *bullet_list)
            {
                mvwaddstr(game_win, bullet->get_yx().first + 1, bullet->get_yx().second + 1, bullet->get_char().c_str());
            }

            // HP and bullet
            wclear(status_win);
            box(status_win, 0, 0);
            mvwprintw(status_win, 1, 1, "%d", (int)(player->get_hp()));

            wrefresh(game_win);
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