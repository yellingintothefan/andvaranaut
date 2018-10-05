#include "Sprite.h"

#include "Speech.h"
#include "util.h"

#include <assert.h>
#include <ctype.h>

static Sprite initspeech(Sprite sprite)
{
    switch(sprite.ascii)
    {
    case 'b':
        sprite.speech = xspsherrif();
        sprite.angers = xspyulia();
        break;
    default:
        sprite.speech = xspzero();
        sprite.angers = xspzero();
        break;
    }
    return sprite;
}

static const Sprite lower[] = {
    { 'a', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'b', 0, 0, 0, 6, 1.0f, 0.04f, 0.0025f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 }, // Outlaw.
    { 'c', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'd', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f,   1.0f, 0.00f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'e', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'f', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'g', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'h', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'i', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'j', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'k', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'l', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'm', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'n', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'o', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'p', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'q', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'r', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 's', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 't', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'u', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'v', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'w', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'x', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'y', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'z', 0, 0, 0, 2, 1.0f, 0.00f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
},
upper[] = {
    { 'A', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'B', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'C', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'D', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'E', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'F', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'G', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'H', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'I', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'J', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'K', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'L', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'M', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'N', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'O', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'P', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'Q', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'R', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'S', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'T', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'U', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'V', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'W', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'X', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'Y', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
    { 'Z', 0, 0, 0, 2, 0.0f, 0.00f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0}, {{0},0,0}, 0 },
};

static void check(const Sprite table[], const int len, const char first, const char last)
{
    for(int i = 0; i < len; i++)
        assert(table[i].ascii == first + i);
    assert(len == last - first + 1);
}

void xstest(void)
{
    check(lower, xlen(lower), 'a', 'z');
    check(upper, xlen(upper), 'A', 'Z');
}

Sprite xsregistrar(const int ascii, const Point where)
{
    Sprite sprite = islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
    sprite.where = sprite.last = where;
    return initspeech(sprite);
}

int xsissprite(const int ascii)
{
    return isalpha(ascii);
}

int xiscosmetic(const int ascii)
{
    return ascii == 'a';
}

int xisinanimate(const int ascii)
{
    return ascii == 'd';
}

int xisuseless(const Sprite* const sprite)
{
    return xisdead(sprite->state) || xiscosmetic(sprite->ascii);
}

int xnocount(const Sprite* const sprite)
{
    return xisuseless(sprite) || xisinanimate(sprite->ascii);
}

int xisstuck(const Sprite* const sprite)
{
    return xisdead(sprite->state) || xishurt(sprite->state);
}

void xsplace(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

int xismute(const Sprite* const sprite)
{
    return sprite->speech.count == 0;
}

void xstick(Sprite* const sprite, const Timer tm)
{
    sprite->speech.ticks = tm.ticks;
}

int xisattack(const Sprite* const sprite)
{
    return (sprite->cooltick % sprite->cooldown) == 0;
}

int xiscontact(const Sprite* const sprite, const Timer tm)
{
    return xisattack(sprite) && tm.fall;
}
