#include "map.h"

namespace game
{
    Wall::Wall(int durability, int y, int x)
    {
        this->durability = durability;
        this->x = x;
        this->y = y;
    }

    std::pair<int, int> Wall::get_yx() { return {y, x}; }

    std::string Wall::get_char()
    {
        if (durability == -1)
            return "1"; // char for permanent wall
        else
            return "0"; // with color
    }

    int Wall::get_durability() { return durability; }
    void Wall::set_durability(int new_durability) { durability = new_durability; }

    Map::Map(std::string map_dir_path)
    {
        map_dir = map_dir_path;
        map = nullptr;
        bitmap = nullptr;
    }

    // Load map: load size and bitmap
    bool Map::load(std::string map_name)
    {
        nlohmann::json json_data;
        std::ifstream file_stream(map_dir + map_name, std::fstream::in);

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

        if (!zb_ent_yx_list.empty())
            zb_ent_yx_list = {};

        _delete_map();

        try
        {
            int lines = json_data["lines"].get<int>();
            int cols = json_data["cols"].get<int>();

            map = new Wall **[lines];
            for (int i = 0; i < lines; i++)
                map[i] = new Wall *[cols];

            bitmap = new char *[lines];
            for (int i = 0; i < lines; i++)
                bitmap[i] = new char[cols];

            int i = 0, j;
            for (auto &line : json_data["bitmap"])
            {
                j = 0;
                for (auto &chr : line.get<std::string>())
                {
                    if (chr == ' ')
                    {
                        map[i][j] = nullptr;
                        bitmap[i][j] = 0;
                    }
                    else if (chr == '#')
                    {
                        map[i][j] = new Wall(-1, i, j);
                        bitmap[i][j] = 1;
                    }
                    j++;
                }
                i++;
            }

            player_init_yx = {json_data["player_init"][0].get<int>(), json_data["player_init"][1].get<int>()};

            // Load zombie entrance points
            for (auto &x : json_data["z_entrance"]["up"])
                zb_ent_yx_list.push_back({0, x.get<int>()});

            for (auto &y : json_data["z_entrance"]["left"])
                zb_ent_yx_list.push_back({y.get<int>(), 0});

            for (auto &x : json_data["z_entrance"]["down"])
                zb_ent_yx_list.push_back({lines - 1, x.get<int>()});

            for (auto &y : json_data["z_entrance"]["right"])
                zb_ent_yx_list.push_back({y.get<int>(), cols - 1});

            this->map_name = map_name;
            this->LINES = lines;
            this->COLS = cols;

            return true;
        }
        catch (const std::exception &e)
        {
            return false;
        }
    }

    void Map::set_variables(std::string map_name, std::vector<std::vector<int>> wall_durabilitys)
    {
        this->load(map_name);
        for (int i = 0; i < LINES; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (wall_durabilitys[i][j] != 0 && map[i][j] == nullptr)
                    add(i, j, wall_durabilitys[i][j]);
            }
        }
    }

    void Map::get_variables(std::string &map_name, std::vector<std::vector<int>> &wall_durabilitys)
    {
        map_name = this->map_name;
        std::vector<int> line;
        for (int i = 0; i < this->LINES; i++)
        {
            line = {};
            for (int j = 0; j < this->COLS; j++)
            {
                if (this->map[i][j] == nullptr)
                    line.push_back(0);
                else
                    line.push_back(this->map[i][j]->get_durability());
            }
            wall_durabilitys.push_back(line);
        }
    }

    std::string Map::get_char(int y, int x)
    {
        if (map[y][x] == NULL)
            return " ";
        else
            return map[y][x]->get_char();
    }

    bool Map::get_bit(int y, int x)
    {
        if (y < 0 || y >= LINES || x < 0 || x >= COLS)
            return 1;
        if (map[y][x] == nullptr)
            return 0;
        return 1;
    }

    bool Map::add(int y, int x, int durability)
    {
        if (map[y][x] == NULL)
        {
            map[y][x] = new Wall(durability, y, x);
            bitmap[y][x] = 1;
            return true;
        }
        return false;
    }

    bool Map::remove(int y, int x)
    {
        if (map[y][x] == NULL)
            return false;
        delete map[y][x];
        map[y][x] = NULL;
        return true;
    }

    bool Map::damage(int y, int x, int dmg)
    {
        if (map[y][x] == nullptr)
            return 0;
        if (map[y][x]->get_durability() != -1)
        {
            map[y][x]->set_durability(map[y][x]->get_durability() - dmg);
            if (map[y][x]->get_durability() <= 0)
                remove(y, x);
        }
        return 0;
    }

    std::pair<int, int> Map::zb_get_rand_ent_yx()
    {
        std::random_device rd;
        int i;
        for (i = (int)((float)(rd() - rd.min()) / (float)(rd.max() - rd.min()) * zb_ent_yx_list.size()); i < 0 || i >= zb_ent_yx_list.size();)
            ;
        return zb_ent_yx_list[i];
    }

    std::pair<int, int> Map::player_get_init_yx() { return player_init_yx; }

    char **Map::get_map() { return bitmap; }

    int Map::columns() { return COLS; }

    int Map::lines() { return LINES; }

    Map::~Map() { _delete_map(); }

    std::vector<std::string> Map::names_of_maps()
    {
        using namespace std::experimental::filesystem;
        using namespace std;

        vector<string> ret;

        path str(map_dir);
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

    std::vector<std::string> Map::minimap(std::string name)
    {

        std::vector<std::string> ret;

        nlohmann::json json_data;
        std::ifstream file_stream(map_dir + name, std::fstream::in);
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

        for (auto &line : json_data["minimap"])
            ret.push_back(line.get<std::string>());

        return ret;
    }

    void Map::_delete_map()
    {
        if (map)
        {
            for (int i = 0; i < LINES; i++)
            {
                for (int j = 0; j < COLS; j++)
                    delete map[i][j];
                delete[] map[i];
            }
            delete[] map;
            map = nullptr;
        }
        if (bitmap)
        {
            for (int i = 0; i < LINES; i++)
                delete[] bitmap[i];
            delete[] bitmap;
            bitmap = nullptr;
        }
    }

}