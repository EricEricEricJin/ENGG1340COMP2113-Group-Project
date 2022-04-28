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

    std::vector<int> Setting::get_keyset()
    {
        try
        {
            std::vector<int> ret(6);
            ret[0] = (*json_data)["keyset"]["up"].get<int>();
            ret[1] = (*json_data)["keyset"]["down"].get<int>();
            ret[2] = (*json_data)["keyset"]["left"].get<int>();
            ret[3] = (*json_data)["keyset"]["right"].get<int>();
            ret[4] = (*json_data)["keyset"]["stop"].get<int>();
            ret[5] = (*json_data)["keyset"]["fire"].get<int>();
            return ret;
        }
        catch (const std::exception &e)
        {
            std::cout << "Get Keyset Error" << std::endl;
            return {};
        }
    }

    Setting::~Setting()
    {
        delete json_data;
    }

}