#ifndef _SETTING_H
#define _SETTING_H

#include <iostream>
#include <fstream>
#include <json.hpp>

namespace game
{
    class Setting
    {
    private:
        nlohmann::json *json_data;

    public:
        Setting()
        {
            // hard-coded boxheadrc path
            json_data = nullptr;
        }

        bool load(std::string boxheadrc_path)
        {
            // Hard coded config file
            std::ifstream f(boxheadrc_path);

            if (f.fail())
            {
                f.close();
                return false;
            }

            if (json_data != nullptr)
                delete json_data;
            json_data = new nlohmann::json;

            try
            {
                f >> *json_data;
                f.close();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
                f.close();
                delete json_data;
                return false;
            }
            return true;
        }

        std::string get_resource_path()
        {
            try
            {
                return (*json_data)["resource_path"].get<std::string>();
            }
            catch (const std::exception &e)
            {
                return "";
            }
        }

        int get_clock_frequency()
        {
            try
            {
                return (*json_data)["clock_frequency"].get<int>();
            }
            catch (const std::exception &e)
            {
                return -1;
            }
        }

        // template <typename T>
        // bool modify(std::string key, T val)
        // {
        //     // @TODO: compare type
        //     (*json_data)[key] = val;
        //     return true;
        // }

        ~Setting()
        {
            delete json_data;
        }
    };
}

#endif