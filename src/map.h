#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <vector>
#include <random>

#include <json.hpp>
#include <fstream>
#include <experimental/filesystem>

// Load map file and add / delete walls.
namespace game
{
    class Wall
    {
    private:
        int durability;
        int type;
        int x;
        int y;

    public:
        Wall(int durability, int y, int x);
        std::pair<int, int> get_yx();
        std::string get_char();
        int get_durability();
        void set_durability(int new_durability);
    };

    class Map
    {
    private:
        Wall ***map;
        char **bitmap;
        int COLS;
        int LINES;
        std::pair<int, int> player_init_yx;
        std::vector<std::pair<int, int>> zb_ent_yx_list;
        std::string map_dir;
        std::string map_name;

    public:
        Map(std::string map_dir_path);
        bool load(std::string map_name); // Load map from file, return whether success

        void set_variables(std::string map_name, std::vector<std::vector<int>> wall_durabilitys);
        void get_variables(std::string &map_name, std::vector<std::vector<int>> &wall_durabilitys);

        std::string get_char(int y, int x); // Get the content in xy, return the content
        bool get_bit(int y, int x);
        bool add(int y, int x, int durability); // Put content in xy, return whether valid
        bool remove(int y, int x);              // Remove content in xy, return whether valid

        bool damage(int y, int x, int dmg);

        std::pair<int, int> zb_get_rand_ent_yx();
        std::pair<int, int> player_get_init_yx();

        char **get_map();

        int columns();
        int lines();
        ~Map();

        std::vector<std::string> names_of_maps();
        std::vector<std::string> minimap(std::string name);
    };
}
#endif