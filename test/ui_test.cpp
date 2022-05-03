#include <iostream>
#include "player.h"
#include "zombie.h"
#include "bullet.h"
#include "map.h"
#include <ui.h>

int main()
{
    
    game::UI ui;
    int difficulty, map_id;
    ui.homepage(&map_id, &difficulty);
    return 0;
}

