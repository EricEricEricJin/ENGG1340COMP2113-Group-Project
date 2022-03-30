#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <vector>

namespace game
{

    class Zombie
    {
    private:
        float x;
        float y;
        std::string character;
    public:
        Zombie();
        std::vector<int> get_xy();
        std::string get_char();
        ~Zombie();
    };

    class zombieKing : public Zombie
    {
    };

    class zombieOrdinary : public Zombie
    {
    };
}
#endif