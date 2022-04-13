#include "player.h"
#include "weapon.h"
#include "map.h"

namespace game
{
    void Player::run()
    {
        running = true;
        std::thread(_player_thread_loop);
    }

    void Player::_player_thread_loop()
    {
        int key;
        while (running)
        {
            key = getch();
            // ...
        }
    }

    void Player::stop()
    {
        running = false;
    }
}