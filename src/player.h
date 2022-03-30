#ifndef _PLAYER_H
#define _PLAYER_H

namespace game
{
    class Player
    {
    private:
        /* data */
    public:
        Player();
        int get_type();          // return type
        int get_hp();            // return hp
        std::string get_char(); // depend on direction
        std::vector<int> get_xy();
        void set_hp(int new_hp); // set hp
        ~Player();
    };

}

#endif