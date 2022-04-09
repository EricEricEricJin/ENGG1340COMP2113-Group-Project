#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <vector>
#include <random>


#include <json.hpp>
#include <fstream>

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
        bool load(std::string fp);          // Load map from file, return whether success
        std::string get_char(int x, int y); // Get the content in xy, return the content
        bool add(int x, int y, int durability); // Put content in xy, return whether valid
        bool remove(int x, int y);          // Remove content in xy, return whether valid
        // int shortest_dir(int x0, int y0, int x1, int y1); // Return the direction to follow shortest path
        
        std::pair<int, int> zb_get_rand_ent_xy();
        
        int columns();
        int lines();
        ~Map();
    };

    std::vector<std::pair<std::string **, std::pair<int, int>>> load_all_maps();
    std::string map_id_to_fp(int map_id);

}
#endif