#include "setting.h"

namespace game
{
    Setting::Setting()
    {
        json_data = nullptr;
    }

    bool Setting::load(std::string boxheadrc_path)
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

    std::string Setting::get_resource_path()
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

    int Setting::get_clock_frequency()
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

    std::vector<int> Setting::get_player_keyset()
    {
        try
        {
            std::vector<int> ret = {0, 0, 0, 0, 0, 0};
            if ((*json_data)["player_keyset"]["up"] != nullptr)
                ret[0] = (*json_data)["player_keyset"]["up"].get<int>();
            if ((*json_data)["player_keyset"]["down"] != nullptr)
                ret[1] = (*json_data)["player_keyset"]["down"].get<int>();
            if ((*json_data)["player_keyset"]["left"] != nullptr)
                ret[2] = (*json_data)["player_keyset"]["left"].get<int>();
            if ((*json_data)["player_keyset"]["right"] != nullptr)
                ret[3] = (*json_data)["player_keyset"]["right"].get<int>();
            if ((*json_data)["player_keyset"]["stop"] != nullptr)
                ret[4] = (*json_data)["player_keyset"]["stop"].get<int>();
            if ((*json_data)["player_keyset"]["fire"] != nullptr)
                ret[5] = (*json_data)["player_keyset"]["fire"].get<int>();
            return ret;
        }
        catch (const std::exception &e)
        {
            std::cout << "Get Keyset Error" << std::endl;
            return {};
        }
    }

    std::vector<int> Setting::get_ui_keyset()
    {
        try
        {
            std::vector<int> ret = {0, 0, 0, 0};
            if ((*json_data)["ui_keyset"]["up"] != nullptr)
                ret[0] = (*json_data)["ui_keyset"]["up"].get<int>();
            if ((*json_data)["ui_keyset"]["down"] != nullptr)
                ret[1] = (*json_data)["ui_keyset"]["down"].get<int>();
            if ((*json_data)["ui_keyset"]["enter"] != nullptr)
                ret[4] = (*json_data)["ui_keyset"]["enter"].get<int>();
            if ((*json_data)["ui_keyset"]["quit"] != nullptr)
                ret[5] = (*json_data)["ui_keyset"]["quit"].get<int>();
            return ret;
        }
        catch (const std::exception &e)
        {
            std::cout << "Get Keyset Error" << std::endl;
            return {};
        }
    }

    std::string Setting::get_theme()
    {
        try
        {
            return (*json_data)["theme"].get<std::string>();
        }
        catch (const std::exception &e)
        {
            return "";
        }
    }

    Setting::~Setting()
    {
        delete json_data;
    }

}