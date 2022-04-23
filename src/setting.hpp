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
        std::string boxheadrc_path;
        nlohmann::json *json_data;

    public:
        Setting()
        {
            // hard-coded boxheadrc path
            boxheadrc_path = getenv("HOME");
            boxheadrc_path += "/.config/boxhead/init.bh";

            json_data = nullptr;
        }

        bool load()
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

        bool save()
        {
            // todo
        }

        template <typename T>
        T query(std::string key)
        {
            return (*json_data)[key].get<T>();
        }

        template <typename T>
        bool modify(std::string key, T val)
        {
            // @TODO: compare type
            (*json_data)[key] = val;
            return true;
        }

        ~Setting()
        {
            delete json_data;
        }
    };
}

#endif