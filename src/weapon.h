#ifndef _WEAPON_H
#define _WEAPON_H

namespace game
{
    class Weapon
    {
    private:
    public:
        Weapon();
        ~Weapon();
    };

    class Pistol : public Weapon
    {
    };

    class machineGun : public Weapon
    {
    };

    class Grenade : public Weapon
    {
    };

    class RPG : public Weapon
    {
    };

    class shotGun : public Weapon
    {
    private:
    public:
    };

    class wallBuilder : public Weapon
    {
    };
}

#endif