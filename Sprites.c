#include "Sprites.h"

#include "Field.h"
#include "Excluding.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <ctype.h>
#include <math.h>

// TODO: Arrays are fine as is. Make a STATE for nulled (where the sprites go red).
// On a map reload, these sprites will be removed.

static Sprite born(const Point where)
{
    Sprite sprite;
    xzero(sprite);
    sprite.where = where;
    sprite.state = IDLE;
    return sprite;
}

/* Clutter Sprite */
// This sprite has no intrinstric properties. States are repurposed for skins.
// The skin is randomly selected.
static Sprite _a_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'a';
    sprite.immovable = true;
    sprite.state = rand() % STATES;
    return sprite;
}

static Sprite _b_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'b';
    sprite.speed = 0.033;
    sprite.acceleration = 0.0025;
    sprite.width = 0.66;
    return sprite;
}

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        default:
        case 'a': return _a_(where); // If the ASCII sprite is not found then default to
        case 'b': return _b_(where); // clutter sprite, though this should never ever happen.
    }
}

Sprites xwake(const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = xconcat("sprites/lvl", which);
    FILE* const file = fopen(path, "r");
    Sprites sprites;
    sprites.count = xlns(file);
    sprites.sprite = xtoss(Sprite, sprites.count);
    sprites.max = sprites.count;
    for(int i = 0; i < sprites.count; i++)
    {
        char* const line = xreadln(file);
        char ascii = 0;
        Point where = { 0.0, 0.0 };
        sscanf(line, "%c,%f,%f\n", &ascii, &where.x, &where.y);
        sprites.sprite[i] = registrar(ascii, where);
        free(line);
    }
    fclose(file);
    free(path);
    return sprites;
}

void xkill(const Sprites sprites)
{
    free(sprites.sprite);
}

Sprites xrewake(const Sprites sprites, const int level)
{
    xkill(sprites);
    return xwake(level);
}

Sprites xlay(Sprites sprites, const Map map, const Overview ov, const int ticks)
{
    // Time delay check. Time dela is arbitrary to feel.
    static int last;
    const int delay = 1;
    if(ticks < last + delay) return sprites;
    last = ticks;
    // Out of bounds check.
    if(xout(map, ov.where)) return sprites;
    // Ascii sprite check.
    const int ascii = ov.selected + ' ';
    if(xissprite(ascii))
    {
        // If the sprite list is empty, resize to one big.
        if(sprites.count == 0)
            xretoss(sprites.sprite, Sprite, sprites.max = 1);
        // If the new sprite cannot fit in the sprite list, resize twice as big.
        if(sprites.count >= sprites.max)
            xretoss(sprites.sprite, Sprite, sprites.max *= 2);
        sprites.sprite[sprites.count++] = registrar(ascii, ov.where);
    }
    return sprites;
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xsub(sprite->where, hero.where);
    }
}

static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xadd(sprite->where, hero.where);
    }
}

static int backwards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return xmag(pa) < xmag(pb) ? +1 : xmag(pa) > xmag(pb) ? -1 : 0;
}

static int forewards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return xmag(pa) < xmag(pb) ? -1 : xmag(pa) > xmag(pb) ? +1 : 0;
}

static void sort(const Sprites sprites, const int foreward)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), foreward ? forewards : backwards);
}

static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = xtoss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

static void turn(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xtrn(sprite->where, -hero.theta);
    }
}

Sprites xorient(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    pull(copied, hero);
    turn(copied, hero);
    sort(copied, false);
    return copied;
}

static void arrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, true);
    push(sprites, hero);
}

static void place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

// Grabs the closest sprite when using hands.
static void grab(const Sprites sprites, const Hero hero, const Input input)
{
    if(!input.l)
        return;
    const Point hand = xtouch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Cannot move immovable sprites.
        if(sprite->immovable)
            continue;
        if(xeql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            xzero(sprite->velocity);
            place(sprite, hand);
            return;
        }
    }
}

// Shoves the closest sprite away if a sprite is grabbed.
static void shove(const Sprites sprites)
{
    // Find the grabbed sprite.
    // Exclude searching for sprites that are clutter sprites.
    Sprite* const grabbed = find(width, sprites, GRABBED);
    if(!grabbed) return;
    // Use the grabbed sprite to shove other sprites.
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const other = &sprites.sprite[i];
        // Ensure the other sprite is not the previously grabbed sprite.
        if(other == grabbed) continue;
        // Do not shove other sprite if other sprite is immovable.
        if(other->width == 0.0) continue;
        // Shove.
        const float width = xmax(other->width, grabbed->width);
        if(xeql(other->where, grabbed->where, width))
        {
            const Point delta = xsub(other->where, grabbed->where);
            place(other, xadd(other->where, delta));
        }
    }
}

// Puts a sprite in the last good spot if the sprite goes into a wall.
// The sprite is stunned and put into a dizzy state.
static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xtile(sprite->where, map.walling))
        {
            place(sprite, xmid(sprite->last));
            xzero(sprite->velocity);
            // TODO: add a dizzy animation.
        }
    }
}

// Moves sprite along force field towards the hero player scent.
static void move(const Sprites sprites, const Field field, const Point to)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Do not move the sprite if the sprite is immovable.
        if(sprite->immovable) continue;
        const Point dir = xforce(field, sprite->where, to);
        // No force of direction...
        if(dir.x == 0.0 && dir.y == 0.0)
            // Then slow down
            sprite->velocity = xmul(sprite->velocity, 1.0 - sprite->acceleration / sprite->speed);
        // Otherwise speed up.
        else
        {
            const Point acc = xmul(dir, sprite->acceleration);
            sprite->velocity = xadd(sprite->velocity, acc);
            // And then check top speed...
            if(xmag(sprite->velocity) > sprite->speed)
                // And cap speed if the top speed is surpassed.
                sprite->velocity = xmul(xunt(sprite->velocity), sprite->speed);
        }
        // If the sprite is fast enough they will animate chase.
        sprite->state = xmag(sprite->velocity) > 0.005 ? CHASING : IDLE;
        // Place the sprite at their new location...
        place(sprite, xadd(sprite->where, sprite->velocity));
    }
}

// Collaborative diffusion with various scents.
static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    const float scent = 1e3;
    // Wall scents are repel.
    for(int j = 0; j < field.rows; j++)
    for(int i = 0; i < field.cols; i++)
    {
        const int y = j / field.res;
        const int x = i / field.res;
        if(map.walling[y][x] != ' ') field.mesh[j][i] = -scent;
    }
    // Sprite scents stack on one another.
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        // If the tile is already scented then a sprite is already there.
        if(field.mesh[j][i] < 0.0) xzero(sprite->velocity);
        field.mesh[j][i] -= sprite->width;
    }
    // Hero scent attracts.
    const int j = field.res * hero.where.y;
    const int i = field.res * hero.where.x;
    field.mesh[j][i] = scent;
    // Diffuse the culminated scent across the field.
    xdiffuse(field, hero.where);
}

bool xissprite(const int ascii)
{
    return isalpha(ascii);
}

void xcaretake(Sprites sprites, const Hero hero, const Input input, const Map map)
{
    // Sprites need to be arrange closest to hero first.
    arrange(sprites, hero);
    // Sprite path finding and movement.
    const Field field = xprepare(map, hero.scent);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where);
    xruin(field);
    // Sprite placement - interactive and out of bounds.
    grab(sprites, hero, input);
    shove(sprites);
    bound(sprites, map);
}
