#include "map.h"

int rand_seed = 0;

int _random_number(int low_b, int high_b)
{
    rand_seed += 1;
    srand(rand_seed);
    // printf("seed: %d", time(0));
    int ret = rand() % (high_b - low_b) + low_b;
    // printf("%d\n", ret);
    return ret;
}

Map::Map(int height, int width)
{
    _map._map = (int **)malloc(sizeof(int *) * height);

    for (int i = 0; i < height; i++)
    {
        *(_map._map + i) = (int *)malloc(sizeof(int) * width);
    }

    _map._map_h = height;
    _map._map_w = width;

    _obstacles = (obstacle_struct *)malloc(sizeof(obstacle_struct) * 10);

    obstacle_width = width / obstacle_num;
}

void Map::setup()
{
    clear_map();
    for (int i = 0; i < obstacle_num; i++)
    {
        create_obstacle(_obstacles + i);
    }
    x_shift = 0;
    first_obstacle_index = 0;
}

void Map::step_fwd()
{
    clear_map();

    for (int i = first_obstacle_index; i < first_obstacle_index + obstacle_num; i++)
    {
        int index = i % obstacle_num;
        draw_line((i - first_obstacle_index) * obstacle_width + (obstacle_width / 2) - x_shift, (_obstacles + index)->empty_y_0, (_obstacles + index)->empty_y_1);
    }

    x_shift += 1;
    if (x_shift >= obstacle_width)
    {
        create_obstacle(_obstacles + first_obstacle_index);
        first_obstacle_index += 1;
        x_shift = 0;
    }

    if (first_obstacle_index >= obstacle_num)
    {
        first_obstacle_index = 0;
    }
}

void Map::create_obstacle(obstacle_struct *obstacle)
{
    obstacle->empty_y_0 = _random_number(0, _map._map_h - bird_height);
    obstacle->empty_y_1 = _random_number(obstacle->empty_y_0 + bird_height, _map._map_h);
}

void Map::draw_line(int x, int y_0, int y_1)
{
    if (x >= 0 and x < _map._map_w)
    {
        for (int row = 0; row < y_0; row++)
        {
            *(*(_map._map + row) + x) = 1;
        }
        for (int row = y_1; row < _map._map_h; row++)
        {
            *(*(_map._map + row) + x) = 1;
        }
    }
}

void Map::clear_map()
{
    for (int row = 0; row < _map._map_h; row++)
    {
        for (int col = 0; col < _map._map_w; col++)
        {
            *(*(_map._map + row) + col) = 0;
        }
    }
}
