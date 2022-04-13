#include "map.h"

namespace game
{

    Wall::Wall(int _durability, int _x, int _y)
    {
        durability = _durability;
        x = _x;
        y = _y;
    }

    std::pair<int, int> Wall::get_xy()
    {
        std::pair<int, int> ret = {x, y};
        return ret;
    }

    std::string Wall::get_char()
    {
        if (durability == -1)
            return "1"; // char for permanent wall
        else
            return "0"; // with color
    }

    Map::Map()
    {
        // allocate map
        std::cout << "Map init" << std::endl;
    }

    // Load map: load size and bitmap
    bool Map::load(std::string fp)
    {
        nlohmann::json json_data;
        std::ifstream file_stream(fp, std::fstream::in);

        // Open resource file
        try
        {
            file_stream >> json_data;
            file_stream.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            file_stream.close();
            return false;
        }

        // Load size
        int lines;
        int cols;

        if (json_data["lines"].is_number_integer())
            lines = json_data["lines"];
        else
            return false;

        if (json_data["cols"].is_number_integer())
            cols = json_data["cols"];
        else
            return false;

        LINES = lines;
        COLS = cols;

        // Load bitmap
        map = (Wall ***)malloc(sizeof(Wall **) * lines); // ?
        for (int i = 0; i < cols; i++)
            map[i] = (Wall **)malloc(sizeof(Wall *) * cols);

        int i, j;

        if (json_data["bitmap"].is_array())
        {
            i = 0;
            for (auto &line : json_data["bitmap"])
            {
                if (line.is_string())
                {
                    j = 0;
                    for (auto &chr : line.get<std::string>())
                    {
                        // std::cout << "in Map::load(): " << i << " " << j << std::endl;
                        if (chr == ' ')
                            map[i][j] = NULL;
                        else if (chr == '#')
                            map[i][j] = new Wall(-1, i, j);
                        j++;
                    }
                    i++;
                }
                else
                    return false;
            }
        }
        else
            return false;

        // Load player initialize point
        if (json_data["player_init"].is_array())
        {
            if (json_data["player_init"][0].is_number_integer() &&
                json_data["player_init"][1].is_number_integer())
            {
                player_init_yx = {
                    json_data["player_init"][0],
                    json_data["player_init"][1]};
            }
            else
                return false;
        }
        else
            return false;

        // Load zombie entrance points
        if (json_data["z_entrance"]["up"].is_array())
        {
            // up: only x coord
            for (auto &x : json_data["z_entrance"]["up"])
                zb_ent_yx_list.push_back({0, x});
        }
        else
            return false;

        if (json_data["z_entrance"]["left"].is_array())
        {
            // left: only y coord
            for (auto &y : json_data["z_entrance"]["left"])
                zb_ent_yx_list.push_back({y, 0});
        }
        else
            return false;

        if (json_data["z_entrance"]["down"].is_array())
        {
            for (auto &x : json_data["z_entrance"]["down"])
                zb_ent_yx_list.push_back({lines - 1, x});
        }
        else
            return false;

        if (json_data["z_entrance"]["right"].is_array())
        {
            for (auto &y : json_data["z_entrance"]["right"])
                zb_ent_yx_list.push_back({y, cols - 1});
        }
        else
            return false;
        return true;
    }

    std::string Map::get_char(int x, int y)
    {
        if (map[y][x] == NULL)
            return " ";
        else
            return map[y][x]->get_char();
    }

    bool Map::add(int x, int y, int durability)
    {
        if (map[y][x] == NULL)
        {
            map[y][x] = new Wall(durability, x, y);
            return true;
        }
        return false;
    }

    bool Map::remove(int x, int y)
    {
        if (map[y][x] == NULL)
            return false;
        delete map[y][x];
        map[y][x] = NULL;
        return true;
    }

    std::pair<int, int> Map::zb_get_rand_ent_xy()
    {
        std::cout << "Size: " << zb_ent_yx_list.size() << std::endl;
        std::random_device rd;
        int i = (int)((float)(rd() - rd.min()) / (float)(rd.max() - rd.min()) * zb_ent_yx_list.size());
        std::cout << "Rand: " << i << std::endl;
        return zb_ent_yx_list[i];
    }

    int Map::columns()
    {
        return COLS;
    }

    int Map::lines()
    {
        return LINES;
    }

    Map::~Map()
    {
        delete[] map;
        // std::cout << "Map Dec" << std::endl;
    }

    std::vector<std::string> Map::names_of_maps()
    {
        using namespace std::filesystem;
        using namespace std;

        vector<string> ret;

        path str(MAP_PATH);
        if (!exists(str))
            return ret;
        directory_entry entry(str);
        if (entry.status().type() != file_type::directory)
            return ret;
        directory_iterator list(str);
        for (auto &i : list)
            ret.push_back(i.path().filename());
        return ret;
    }

    std::vector<std::vector<std::string>> Map::minimap(std::string name)
    {

        std::vector<std::vector<std::string>> ret;

        nlohmann::json json_data;
        std::ifstream file_stream(MAP_PATH + name, std::fstream::in);
        try
        {
            file_stream >> json_data;
            file_stream.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            file_stream.close();
            return ret;
        }

        // json_data["minimap"]
        return ret;
    }

}