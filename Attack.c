#include "Attack.h"

#include "Util.h"
#include "Hero.h"
#include "Point.h"

static float reach(const Weapon weapon)
{
    switch(weapon)
    {
        case LONG_SWORD: return 0.75;
        default:
            return 0.0;
    }
}

static float power(const Weapon weapon)
{
    switch(weapon)
    {
        case LONG_SWORD: return 0.10;
        default:
            return 0.0;
    }
}

extern Attack nothing()
{
    Attack attack;
    zero(attack);
    return attack;
}

extern Attack swing(const Hero hero, const Point vect)
{
    Attack attack = nothing();
    attack.type.swing = true;
    attack.vect = vect;
    attack.power = power(hero.weapon) * mag(vect);
    attack.reach = hero.arm + reach(hero.weapon);
    attack.where = touch(hero, attack.reach);
    attack.area = 2 * attack.reach;
    return attack;
}
