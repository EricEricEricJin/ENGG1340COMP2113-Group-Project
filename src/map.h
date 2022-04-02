#ifndef _MAP_H
#define _MAP_H

#include <iostream>
#include <vector>

#include <json.hpp>
#include <fstream>

// Load map file and add / delete walls.
namespace game
{
    class Wall
    {
    public:
        std::vector<int> get_xy();
        int get_type();
        std::string get_char();
    };

    class Map
    {
    private:
        Wall ***map;
        int COLS;
        int LINES;

    public:
        Map(int _lines, int _cols);
        bool load(std::string fp);                        // Load map from file, return whether success
        std::string get_char(int x, int y);               // Get the content in xy, return the content
        bool add(int x, int y, Wall *wall);               // Put content in xy, return whether valid
        bool remove(int x, int y);                        // Remove content in xy, return whether valid
        // int shortest_dir(int x0, int y0, int x1, int y1); // Return the direction to follow shortest path
        int columns();
        int rows();
        ~Map();
    };

}
#endif