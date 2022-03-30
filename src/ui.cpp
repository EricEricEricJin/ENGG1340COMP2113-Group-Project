#include "ui.h"

namespace game
{
    void UI::start_game(std::vector<Player> &_player_list, std::vector<Zombie> &_zombie_list, Map &_map)
    {
        // set dependencies
        player_list = &_player_list;
        zombie_list = &_zombie_list;
        map = &_map;
    }

    void UI::refresh()
    {
        clear();
        _draw_walls();
        _draw_zombies();
        _draw_bullets();
        _draw_players();
        refresh();
    }

    void UI::_draw_walls()
    {
        for (int r = 0; r < map->rows(); r++)
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

}
