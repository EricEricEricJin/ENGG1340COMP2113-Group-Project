#ifndef _PLAYER_H
#define _PLAYER_H

namespace game
{
    class player
    {
    private:
        /* data */
    public:
        player();
        int get_type();          // return type
        int get_hp();            // return hp
        void set_hp(int new_hp); // set hp
        ~player();
    };

}

#endif