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
#include "rw_saved.h"

namespace game
{
    UI::UI() {}

    bool UI::init(Player *player, std::list<Zombie *> *zombie_list, std::list<Bullet *> *bullet_list, Map *map, Clock *clock, rwSaved *rw_saved)
    {
        this->player = player;
        this->zombie_list = zombie_list;
        this->bullet_list = bullet_list;
        this->map = map;
        this->clock = clock;
        this->rw_saved = rw_saved;

        initscr();
        noecho();
        nocbreak();
        timeout(0);
        curs_set(0);

        if (LINES < WIN_HEIGHT || COLS < WIN_WIDTH)
        {
            endwin();
            return false;
        }
        return true;

        // box(stdscr, 0, 0);
        // refresh();
    }

    void UI::configure(uiKeySet keyset, int theme)
    {
        this->keyset = keyset;
        this->theme = theme;

        start_color();
        if (theme & UTHEME_LIGHT)
        {
            init_pair(UCOLOR_BOX, COLOR_BLACK, COLOR_WHITE);
            init_pair(UCOLOR_MENU, COLOR_BLACK, COLOR_WHITE);
            init_pair(UCOLOR_PLAYER, COLOR_BLUE, COLOR_WHITE);
            init_pair(UCOLOR_ZOMBIE, COLOR_GREEN, COLOR_WHITE);
            init_pair(UCOLOR_WALL, COLOR_BLACK, COLOR_WHITE);
            init_pair(UCOLOR_BULLET, COLOR_YELLOW, COLOR_WHITE);

            init_pair(UCOLOR_HP_L, COLOR_RED, COLOR_WHITE);
            init_pair(UCOLOR_HP_M, COLOR_YELLOW, COLOR_WHITE);
            init_pair(UCOLOR_HP_H, COLOR_GREEN, COLOR_WHITE);
        }
        else if (theme & UTHEME_DARK)
        {
            init_pair(UCOLOR_BOX, COLOR_GREEN, COLOR_BLACK);
            init_pair(UCOLOR_MENU, COLOR_GREEN, COLOR_BLACK);
            init_pair(UCOLOR_PLAYER, COLOR_BLUE, COLOR_BLACK);
            init_pair(UCOLOR_ZOMBIE, COLOR_YELLOW, COLOR_BLACK);
            init_pair(UCOLOR_WALL, COLOR_GREEN, COLOR_BLACK);
            init_pair(UCOLOR_BULLET, COLOR_WHITE, COLOR_BLACK);

            init_pair(UCOLOR_HP_L, COLOR_RED, COLOR_BLACK);
            init_pair(UCOLOR_HP_M, COLOR_YELLOW, COLOR_BLACK);
            init_pair(UCOLOR_HP_H, COLOR_GREEN, COLOR_BLACK);
        }
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
        wbkgd(home_win, COLOR_PAIR(UCOLOR_MENU));

        std::array<std::string, 5> items = {"New game", "Load saving", "Setting", "Edit map", "Exit"};
        int current_item = 0;

        while (true)
        {
            wclear(home_win);
            wattron(home_win, COLOR_PAIR(UCOLOR_BOX));
            box(home_win, 0, 0);
            wattroff(home_win, COLOR_PAIR(UCOLOR_BOX));

            // display menu
            wattron(home_win, COLOR_PAIR(UCOLOR_MENU));
            for (int i = 0; i < items.size(); i++)
            {
                if (current_item == i)
                    wattron(home_win, A_REVERSE);
                mvwaddstr(home_win, WIN_HEIGHT / items.size() * i + 1, WIN_WIDTH / 2 + 1, items[i].c_str());

                if (current_item == i)
                    wattroff(home_win, A_REVERSE);
            }
            wattroff(home_win, COLOR_PAIR(UCOLOR_MENU));
            wrefresh(home_win);

            int _key = wgetch(home_win);
            if (_key == keyset.key_down)
                current_item++;
            else if (_key == keyset.key_up)
                current_item--;
            else if (_key == keyset.key_enter)
            {
                werase(home_win);
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
                    bool _ret = _load_saving_page(ret_string);
                    if (_ret)
                    {
                        *ret_kind = HOMEPAGE_LOAD;
                        break;
                    }
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
        WINDOW *list_win = newwin(WIN_HEIGHT, 20, WIN_OFFSET_Y, WIN_OFFSET_X);
        wbkgd(list_win, COLOR_PAIR(UCOLOR_MENU));

        wattron(list_win, COLOR_PAIR(UCOLOR_BOX));
        box(list_win, 0, 0);
        wattroff(list_win, COLOR_PAIR(UCOLOR_BOX));

        WINDOW *preview_win = nullptr;

        auto map_names = map->names_of_maps();

        int map_idx = 0;

        bool ret;

        while (true)
        {
            // display name list
            wattron(list_win, COLOR_PAIR(UCOLOR_MENU));
            for (int i = 0; i < map_names.size(); i++)
            {
                if (map_idx == i)
                    wattron(list_win, A_REVERSE);
                mvwaddstr(list_win, i + 1, 1, map_names[i].c_str());
                if (map_idx == i)
                    wattroff(list_win, A_REVERSE);
            }
            wattroff(list_win, COLOR_PAIR(UCOLOR_MENU));
            wrefresh(list_win);

            // draw minimap
            auto minimap = map->minimap(map_names[map_idx]);
            if (preview_win != nullptr)
                delwin(preview_win);
            preview_win = newwin(minimap.size() + 2, minimap[0].length() + 2, WIN_OFFSET_Y, WIN_OFFSET_X + 20);
            wbkgd(preview_win, COLOR_PAIR(UCOLOR_MENU));

            wattron(preview_win, COLOR_PAIR(UCOLOR_MENU));
            box(preview_win, 0, 0);
            wattroff(preview_win, COLOR_PAIR(UCOLOR_MENU));

            wattron(preview_win, COLOR_PAIR(UCOLOR_MENU));
            int x, y = 1;
            for (auto &line : minimap)
            {
                x = 1;
                for (auto &chr : line)
                {
                    if (chr != ' ')
                        mvwaddch(preview_win, y, x, ACS_BLOCK);
                    x++;
                }
                y++;
            }
            wattroff(preview_win, COLOR_PAIR(UCOLOR_MENU));
            wrefresh(preview_win);

            int _key = wgetch(list_win);
            if (_key == keyset.key_down)
                map_idx++;
            else if (_key == keyset.key_up)
                map_idx--;
            else if (_key == keyset.key_enter)
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

    bool UI::_load_saving_page(std::string *ret_string)
    {
        auto saving_list = rw_saved->get_all_savings();
        auto ret = _select_list(saving_list);
        if (ret == -1)
            return false;
        *ret_string = saving_list[ret];
        return true;
    }

    void UI::_setting_page() {}
    void UI::_edit_map_page() {}

    void UI::start_game()
    {
        int y_0 = (LINES - map->lines() - 3) / 2;
        int width = (map->columns() + 2);
        int x_0 = (COLS - width) / 2;

        status_win = newwin(1, width, y_0, x_0);
        wbkgd(status_win, COLOR_PAIR(UCOLOR_MENU));
        game_win = newwin(map->lines() + 2, width, y_0 + 1, x_0);
        wbkgd(game_win, COLOR_PAIR(UCOLOR_WALL));

        // start thread...
        status_val = USTATUS_RUNNING;
        thread_obj = new std::thread([=]
                                     { _game_thread_loop(); });
    }

    void UI::_game_thread_loop()
    {
        // keypad(game_win, true);
        // keypad(stdscr, true);

        // draw and refresh screen
        while (status_val == USTATUS_RUNNING)
        {

            werase(game_win);

            wattron(game_win, COLOR_PAIR(UCOLOR_WALL));
            // this is wall
            if (theme & UTHEME_THIN)
                box(game_win, 0, 0);
            else
                wborder(game_win, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD, ACS_CKBOARD);
            wattroff(game_win, COLOR_PAIR(UCOLOR_WALL));

            // map
            // goto DRAW_MAP_END;
            wattron(game_win, COLOR_PAIR(UCOLOR_WALL));
            for (int i = 0; i < map->lines(); i++)
            {
                for (int j = 0; j < map->columns(); j++)
                {
                    if (map->get_bit(i, j))
                    {
                        chtype wall_chr;
                        if (theme & UTHEME_THIN)
                        {
                            int srding = (map->get_bit(i - 1, j) << 3) | (map->get_bit(i + 1, j) << 2) | (map->get_bit(i, j - 1) << 1) | map->get_bit(i, j + 1);

                            if ((srding & 0b1100) && !(srding & 0b0011))
                                wall_chr = ACS_VLINE;
                            else if ((srding & 0b0011) && !(srding & 0b1100))
                                wall_chr = ACS_HLINE;
                            else if (srding == 0b0101)
                                wall_chr = ACS_ULCORNER;
                            else if (srding == 0b0110)
                                wall_chr = ACS_URCORNER;
                            else if (srding == 0b1001)
                                wall_chr = ACS_LLCORNER;
                            else if (srding == 0b1010)
                                wall_chr = ACS_LRCORNER;
                            else if (srding == 0b1101)
                                wall_chr = ACS_LTEE;
                            else if (srding == 0b1110)
                                wall_chr = ACS_RTEE;
                            else if (srding == 0b0111)
                                wall_chr = ACS_TTEE;
                            else if (srding == 0b1011)
                                wall_chr = ACS_BTEE;
                            else if (srding == 0b1111)
                                wall_chr = ACS_PLUS;
                        }
                        else
                            wall_chr = ACS_CKBOARD;

                        mvwaddch(game_win, i + 1, j + 1, wall_chr);
                    }
                }
            }
            wattroff(game_win, COLOR_PAIR(UCOLOR_WALL));

            // player
            wattron(game_win, COLOR_PAIR(UCOLOR_PLAYER));
            mvwaddstr(game_win, (int)round(player->get_yx().first + 1), (int)round(player->get_yx().second + 1), player->get_char().c_str());
            wattroff(game_win, COLOR_PAIR(UCOLOR_PLAYER));

            // zombie
            wattron(game_win, COLOR_PAIR(UCOLOR_ZOMBIE));
            for (auto &zombie : *zombie_list)
            {
                mvwaddstr(game_win, (int)round(zombie->get_yx().first) + 1, (int)round(zombie->get_yx().second) + 1, zombie->get_char().c_str());
            }
            wattroff(game_win, COLOR_PAIR(UCOLOR_ZOMBIE));

            // bullet
            wattron(game_win, COLOR_PAIR(UCOLOR_BULLET));
            for (auto &bullet : *bullet_list)
            {
                mvwaddstr(game_win, (int)round(bullet->get_yx().first) + 1, (int)round(bullet->get_yx().second) + 1, bullet->get_char().c_str());
            }
            wattroff(game_win, COLOR_PAIR(UCOLOR_BULLET));

            wrefresh(game_win);

            // HP and bullet
            werase(status_win);

            wattron(status_win, COLOR_PAIR(UCOLOR_MENU) | A_REVERSE);
            mvwprintw(status_win, 0, 0, "    HP    ");
            mvwprintw(status_win, 0, 25, "  WEAPON  ");
            mvwprintw(status_win, 0, 50, "   SCORE  ");
            wattroff(status_win, COLOR_PAIR(UCOLOR_MENU) | A_REVERSE);

            wattron(status_win, COLOR_PAIR(UCOLOR_MENU));
            mvwprintw(status_win, 0, 35, "  %s", player->get_cur_bul_name().c_str());
            mvwprintw(status_win, 0, 60, "  %ld", clock->get_ticks());
            wattroff(status_win, COLOR_PAIR(UCOLOR_MENU));

            int hp_color;
            if (player->get_hp() > 66.6)
                hp_color = COLOR_PAIR(UCOLOR_HP_H);
            else if (player->get_hp() > 33.3)
                hp_color = COLOR_PAIR(UCOLOR_HP_M);
            else
                hp_color = COLOR_PAIR(UCOLOR_HP_L);

            wattron(status_win, hp_color);
            mvwprintw(status_win, 0, 10, "  %d", (int)(player->get_hp()));
            wattroff(status_win, hp_color);

            wrefresh(status_win);

            // deal with menu
            if (key == 'p')
            {
                status_val = USTATUS_PAUSE;
                wclear(game_win);
                wrefresh(game_win);
                wclear(status_win);
                wrefresh(status_win);

                if (auto menu_ret = _game_menu(); menu_ret == 0) // resume
                {
                    status_val = USTATUS_RUNNING;
                }
                else if (menu_ret == 1) // save
                {
                    auto saving_name = _prompt_input("Saving name", 16);
                    rw_saved->get_write(saving_name);
                    status_val = USTATUS_EXIT;
                    break;
                }
                else if (menu_ret == 2) // exit
                {
                    status_val = USTATUS_EXIT;
                    break;
                }
            }

            // key = wgetch(game_win);
            clock->wait(1);
            key = getch();
        }
        delwin(game_win);
        delwin(status_win);
    }

    int UI::_game_menu()
    {
        cbreak();
        timeout(-1);

        WINDOW *menu_win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X);
        wbkgd(menu_win, COLOR_PAIR(UCOLOR_MENU));

        std::array<std::string, 3> items = {"Resume", "Save", "Exit"};
        int current_item = 0;

        // display
        while (true)
        {
            wclear(menu_win);
            wattron(menu_win, COLOR_PAIR(UCOLOR_BOX));
            box(menu_win, 0, 0);
            wattroff(menu_win, COLOR_PAIR(UCOLOR_BOX));

            wattron(menu_win, COLOR_PAIR(UCOLOR_MENU));

            for (int i = 0; i < items.size(); i++)
            {
                if (i == current_item)
                    wattron(menu_win, A_REVERSE);
                mvwaddstr(menu_win, WIN_HEIGHT / items.size() * i + 1, WIN_WIDTH / 2 + 1, items[i].c_str());
                if (i == current_item)
                    wattroff(menu_win, A_REVERSE);
            }
            wattroff(menu_win, COLOR_PAIR(UCOLOR_MENU));

            wrefresh(menu_win);
            int key = getch();
            if (key == keyset.key_down)
                current_item++;
            else if (key == keyset.key_up)
                current_item--;
            else if (key == keyset.key_enter)
                break;

            if (current_item < 0)
                current_item = items.size() - 1;
            else if (current_item >= items.size())
                current_item = 0;
        }

        delwin(menu_win);
        nocbreak();
        timeout(0);
        return current_item;
    }

    void UI::stop_game()
    {
        status_val = USTATUS_EXIT;
        // if (thread_obj->joinable())
        thread_obj->join();

        delete thread_obj;

        // how to stop?
    }

    int UI::get_status() { return status_val; }

    int UI::_select_list(std::vector<std::string> option_list)
    {
        timeout(-1);
        WINDOW *list_win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X);
        box(list_win, 0, 0);
        int option = 0;

        int menu_key = 0;

        int max_len = 0;
        for (auto &i : option_list)
            if (i.length() > max_len)
                max_len = i.length();

        int first_y = (WIN_HEIGHT - option_list.size()) / 2;
        int first_x = (WIN_WIDTH - max_len) / 2;

        for (;;)
        {
            // display
            werase(list_win);
            
            wattron(list_win, COLOR_PAIR(UCOLOR_BOX));
            box(list_win, 0, 0);
            wattroff(list_win, COLOR_PAIR(UCOLOR_BOX));
            
            wattron(list_win, COLOR_PAIR(UCOLOR_MENU));
            for (int i = 0; i < option_list.size(); i++)
            {
                if (i == option)
                    wattron(list_win, A_REVERSE);
                mvwprintw(list_win, first_y + i, first_x, option_list[i].c_str());
                if (i == option)
                    wattroff(list_win, A_REVERSE);
            }
            wattroff(list_win, COLOR_PAIR(UCOLOR_MENU));
            wrefresh(list_win);

            menu_key = getch();
            if (menu_key == keyset.key_enter)
                break;
            else if (menu_key == keyset.key_quit)
            {
                option = -1;
                break;
            }
            else if (menu_key == keyset.key_up)
                option--;
            else if (menu_key == keyset.key_down)
                option++;

            if (option < 0)
                option = option_list.size() - 1;
            else if (option >= option_list.size())
                option = 0;
        }

        delwin(list_win);
        return option;
    }

    std::string UI::_prompt_input(std::string prompt, int max_len)
    {
        WINDOW *prompt_win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X);
        echo();
        timeout(-1);

        wattron(prompt_win, COLOR_PAIR(UCOLOR_BOX));
        box(prompt_win, 0, 0);
        wattroff(prompt_win, COLOR_PAIR(UCOLOR_BOX));

        mvwprintw(prompt_win, WIN_HEIGHT / 3, (WIN_WIDTH - prompt.length()) / 2, prompt.c_str());

        char *input_str = new char[max_len + 1];
        mvwgetnstr(prompt_win, WIN_HEIGHT / 3 * 2, (WIN_WIDTH - max_len) / 2, input_str, max_len);

        std::string ret_str(input_str);
        delete[] input_str;
        delwin(prompt_win);
        noecho();
        timeout(0);
        return ret_str;
    }

    UI::~UI() {}

}