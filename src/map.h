#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <vector>
#include <random>

#include <json.hpp>
#include <fstream>
#include <experimental/filesystem>

#define MAP_PATH "../resource/map/"

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
        Wall(int _durability, int _x, int _y);
        std::pair<int, int> get_xy();
        std::string get_char();
    };

    class Map
    {
    private:
        Wall ***map;
        int COLS;
        int LINES;
        std::pair<int, int> player_init_yx;
        std::vector<std::pair<int, int>> zb_ent_yx_list;

    public:
        Map();
        bool load(std::string fp);              // Load map from file, return whether success
        std::string get_char(int x, int y);     // Get the content in xy, return the content
        bool add(int x, int y, int durability); // Put content in xy, return whether valid
        bool remove(int x, int y);              // Remove content in xy, return whether valid
        // int shortest_dir(int x0, int y0, int x1, int y1); // Return the direction to follow shortest path

        std::pair<int, int> zb_get_rand_ent_xy();

        int columns();
        int lines();
        ~Map();

        static std::vector<std::string> names_of_maps();
        static std::vector<std::vector<std::string>> minimap(std::string name);
    };
}
#endif