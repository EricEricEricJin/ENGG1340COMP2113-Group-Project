#include "map.h"

namespace game
{
    Map::Map(int _lines, int _cols)
    {
        LINES = _lines;
        COLS = _cols;

        // allocate map
        map = (Wall ***)malloc(sizeof(Wall **) * LINES);
        for (int i = 0; i < LINES; i++)
        {
            map[i] = (Wall **)malloc(sizeof(Wall *) * COLS);
            for (int j = 0; j < COLS; j++)
                map[i][j] = NULL;
        }
    }

    bool Map::load(std::string fp)
    {
        std::vector<std::vector<float>> vec;

        nlohmann::json json_data;
        std::ifstream file_stream(fp, std::fstream::in);

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

        if (!json_data.is_array())
            return false;

        for (auto &line : json_data)
        {
            if (!line.is_array())
                return false;
            std::vector<float> l;
            for (auto &number : line)
            {
                if (number.is_number_float())
                    l.push_back(number.get<float>());
                else
                    return false;
            }
            vec.push_back(l);
        }

        // build bitmap
        for (auto &line : vec)
            for (int x = line[0] * COLS; x <= line[2] * COLS; x++)
                for (int y = line[1] * LINES; y < line[3] * LINES; y++)
                    map[y][x] = new Wall();

        return true;
    }

    std::string Map::get_char(int x, int y)
    {
        if (map[y][x] == NULL)
            return " ";
        else
            return map[y][x]->get_char();
    }

    bool Map::add(int x, int y, Wall *wall)
    {
        if (map[y][x] == NULL)
        {
            map[y][x] = wall;
            return true;
        }
        return false;
    }

    bool Map::remove(int x, int y)
    {
        if (map[y][x] == NULL)
            return false;
        delete map[y][x];
        return true;
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
        for (int i = 0; i < LINES; i++)
        {
            for (int j = 0; j < COLS; j++)
                free(map[i][j]);
            free(map[i]);
        }
    }

}