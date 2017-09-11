#pragma once

#include "Hero.h"
#include "State.h"

typedef struct
{
    Point where;
    int ascii;
    State state;
    int transparent;
    float width;
    float health;
    int ticks;
    int moveable;
    Point last;
}
Sprite;

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
}
Sprites;

Sprites wake(const int level);

void kill(const Sprites sprites);

Sprites rewake(const Sprites sprites, const int level);

Sprites arrange(const Sprites sprites, const Hero hero);

bool issprite(const int ascii);

Sprites caretake(const Sprites sprites, const Hero hero, const Input input,
    const Map map, const int ticks);
