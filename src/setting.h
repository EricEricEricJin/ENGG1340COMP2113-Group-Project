#ifndef _SETTING_H
#define _SETTING_H

#include <iostream>

namespace game
{
    class Setting
    {
    private:
    public:
        Setting();
        bool load(std::string file_name);
        bool save(std::string file_name);
        bool query(std::string key, void *value);
        bool modify(std::string key, void *value);
        ~Setting();
    };
}

#endif