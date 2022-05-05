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
        cbreak();
        timeout(0);
        curs_set(0);

        thread_obj = nullptr;

        if (LINES < WIN_HEIGHT || COLS < WIN_WIDTH)
        {
            endwin();
            return false;
        }
        WIN_OFFSET_Y = (LINES - WIN_HEIGHT) / 2;
        WIN_OFFSET_X = (COLS - WIN_WIDTH) / 2;

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
        timeout(-1);

        WINDOW *home_win = newwin(24, 80, WIN_OFFSET_Y, WIN_OFFSET_X);
        wbkgd(home_win, COLOR_PAIR(UCOLOR_MENU));

        std::vector<std::string> items = {"New game", "Load saving", "Exit"};

        if (int current_item = _select_list(items, WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X); current_item == 0)
        {
            bool _ret = _new_game_page(ret_string);
            if (_ret)
                *ret_kind = HOMEPAGE_NEWG;
        }
        else if (current_item == 1)
        {
            bool _ret = _load_saving_page(ret_string);
            if (_ret)
                *ret_kind = HOMEPAGE_LOAD;
        }
        else if (current_item == 2 || current_item == -1)
            *ret_kind = HOMEPAGE_EXIT;

        delwin(home_win);
        timeout(0);
        return false;
    }

    bool UI::_new_game_page(std::string *map_name)
    {
        WINDOW *preview_win = nullptr;

        auto map_names = map->names_of_maps();

        bool ret = true;
        int current_item = 0;

        bool disp_mm_running = true;

        // display map thread
        std::thread display_minimap_thread([this, &preview_win, &map_names, &current_item, &disp_mm_running]
                                           {
            int current_current_item = current_item;
            while (disp_mm_running)
            {
                if (current_current_item != current_item)
                {
                    current_current_item = current_item;
                    auto minimap = map->minimap(map_names[current_item]);
                    if (preview_win != nullptr)
                    {
                        werase(preview_win);
                        wrefresh(preview_win);
                        delwin(preview_win);
                        preview_win = nullptr;
                    }
                    preview_win = newwin(minimap.size() + 2, minimap[0].length() + 2, WIN_OFFSET_Y + (WIN_HEIGHT - minimap.size() - 2) / 2,
                                        WIN_OFFSET_X + WINT_WIDTH / 2 + (WIN_WIDTH - minimap[0].size() - 2) / 2);
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
                }
                else
                    clock->wait(1);
            } });

        if (current_item = _select_list(map_names, WIN_HEIGHT, WIN_WIDTH / 2, WIN_OFFSET_Y, WIN_OFFSET_X, &current_item); current_item == -1)
            ret = false;
        else
            *map_name = map_names[current_item];

        disp_mm_running = false;
        display_minimap_thread.join();

        delwin(preview_win);
        return ret;
    }

    bool UI::_load_saving_page(std::string *ret_string)
    {
        auto saving_list = rw_saved->get_all_savings();
        auto ret = _select_list(saving_list, WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X);
        if (ret == -1)
            return false;
        *ret_string = saving_list[ret];
        return true;
    }

    // void UI::_setting_page() {}
    // void UI::_edit_map_page() {}

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
                werase(game_win);
                wrefresh(game_win);
                werase(status_win);
                wrefresh(status_win);

                std::vector<std::string> menu_items = {"Resume", "Save and Exit", "Exit"};

                if (int current_item = _select_list(menu_items, WIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X); current_item == 0 || current_item == -1) // resume
                {
                    status_val = USTATUS_RUNNING;
                }
                else if (current_item == 1) // save
                {
                    auto saving_name = _prompt_input("Saving name", 16);
                    rw_saved->get_write(saving_name);
                    status_val = USTATUS_EXIT;
                }
                else if (current_item == 2) // exit
                {
                    status_val = USTATUS_EXIT;
                }
            }

            // key = wgetch(game_win);
            clock->wait(1);
            key = getch();
        }

        delwin(game_win);
        delwin(status_win);
    }

    void UI::stop_game()
    {
        status_val = USTATUS_EXIT;
        if (thread_obj && thread_obj->joinable())
        {
            thread_obj->join();
            delete thread_obj;
            thread_obj = nullptr;
        }
    }

    int UI::get_status() { return status_val; }

    int UI::_select_list(std::vector<std::string> option_list, int height, int width, int y_beg, int x_beg, int *item_ptr)
    {
        timeout(-1);

        WINDOW *list_win = newwin(height, width, y_beg, x_beg);
        wbkgd(list_win, COLOR_PAIR(UCOLOR_MENU));
        int option = 0;

        int menu_key = 0;

        int max_len = 0;
        for (auto &i : option_list)
            if (i.length() > max_len)
                max_len = i.length();

        int first_y = (height - option_list.size()) / 2;
        int first_x = (width - max_len) / 2;

        wattron(list_win, COLOR_PAIR(UCOLOR_MENU));
        for (;;)
        {
            if (item_ptr)
                *item_ptr = option;

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

            menu_key = wgetch(list_win);
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
        wattroff(list_win, COLOR_PAIR(UCOLOR_MENU));

        werase(list_win);
        wrefresh(list_win);
        delwin(list_win);

        timeout(0);

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
        werase(prompt_win);
        wrefresh(prompt_win);
        delwin(prompt_win);
        noecho();
        timeout(0);
        return ret_str;
    }

    UI::~UI() { stop_game(); }

}