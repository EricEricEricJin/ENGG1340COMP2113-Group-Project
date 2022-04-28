#ifndef _SETTING_H
#define _SETTING_H

#include <iostream>
#include <fstream>
#include <json.hpp>
#include <vector>

namespace game
{
    class Setting
    {
    private:
        nlohmann::json *json_data;

    public:
        Setting();

        bool load(std::string boxheadrc_path);
        std::string get_resource_path();

        int get_clock_frequency();

        std::vector<int> get_keyset();

        ~Setting();
    };
}

#endif