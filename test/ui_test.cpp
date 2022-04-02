#include <iostream>
#include "player.h"
#include "zombie.h"
#include "bullet.h"
#include "map.h"
#include <ui.h>

int main()
{
    game::Zombie zombie;
    game::Player player;
    game::UI ui;

    ui.start_game();
    ui.refresh();

}

