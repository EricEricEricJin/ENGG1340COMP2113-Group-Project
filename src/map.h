#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>

struct map_struct
{
    int **_map;
    int _map_h;
    int _map_w;
};

struct obstacle_struct
{
    int empty_y_0;
    int empty_y_1;
};

class Map
{
public:
    Map(int height, int width);
    void setup();
    void step_fwd();
    map_struct _map;

private:
    const int obstacle_num = 6;

    int obstacle_width;

    const int bird_height = 4;

    obstacle_struct *_obstacles;

    int x_shift;

    int first_obstacle_index;

    void create_obstacle(obstacle_struct *obstacle);

    void draw_line(int x, int y_0, int y_1);

    void clear_map();
};