#include "Scanline.h"

#include "util.h"

static uint32_t shade(const uint32_t pixel, const int shading)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * shading) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * shading) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * shading) >> 0x08; // gcc -Ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

static uint32_t pget(const SDL_Surface* const surface, const Point offset)
{
    const int row = surface->h * xdec(offset.y);
    const int col = surface->w * xdec(offset.x);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    return pixels[col + row * surface->w];
}

static void pput(const Scanline sl, const int x, const uint32_t pixel)
{
    sl.pixels[x + sl.y * sl.width] = pixel;
}

static void xfer(const Scanline sl, const int x, const Point offset, const int tile, const int distance)
{
    const uint32_t color = pget(sl.sdl.surfaces.surface[tile], offset);
    pput(sl, x, shade(color, distance));
}

static void rwall(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.offset };
        xfer(sl, x, offset, r.surface, xilluminate(r.torch, r.corrected.x));
    }
}

static void rflor(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        const int tile = p_tile(offset, map.floring);
        if(!tile)
            continue;
        xfer(sl, x, offset, tile, xilluminate(r.torch, p_mag(p_sub(offset, r.trace.a))));
    }
}

static void rceil(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        const Point offset = xlerp(r.trace, xccast(r.proj, x));
        const int tile = p_tile(offset, map.ceiling);
        if(!tile)
            continue;
        xfer(sl, x, offset, tile, xilluminate(r.torch, p_mag(p_sub(offset, r.trace.a))));
    }
}

static void rsky(const Scanline sl, const Ray r, const Map map, const int floor, const Flow clouds)
{
    if(floor == 0)
    {
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const Sheer sa = { 0.0f, clouds.height };
            const Point a = xlerp(r.trace, xccast(xsheer(r.proj, sa), x));
            xfer(sl, x, p_div(p_abs(p_sub(a, clouds.where)), 8.0f), '&' - ' ', xilluminate(r.torch, p_mag(p_sub(a, r.trace.a))));
        }
    }
    else
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const Point offset = xlerp(r.trace, xccast(r.proj, x));
            if(p_tile(offset, map.ceiling))
                continue;
            xfer(sl, x, offset, '#' - ' ', xilluminate(r.torch, p_mag(p_sub(offset, r.trace.a))));
        }
}

static void rpit(const Scanline sl, const Ray r, const Map map, const Flow current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        if(p_tile(offset, map.floring))
            continue;
        xfer(sl, x, p_abs(p_sub(offset, current.where)), '%' - ' ', xilluminate(r.torch, p_mag(p_sub(offset, r.trace.a))));
    }
}

static void rupper(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow clouds)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Ray ray = xcalc(hero, *which, map.top, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            rsky(sl, ray, map, hero.floor, clouds);
        rwall(sl, ray);
    }
}

static void rlower(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow current)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Sheer sheer = { current.height, -1.0f };
        const Ray ray = xcalc(hero, *which, sheer, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            rpit(sl, ray, map, current);
        rwall(sl, ray);
    }
}

static Point rmiddle(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = xcalc(hero, hits.walling, map.mid, sl.sdl.yres, sl.sdl.xres);
    rwall(sl, ray);
    rflor(sl, ray, map);
    rceil(sl, ray, map);
    return ray.corrected;
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Flow current, const Flow clouds, const Map map)
{
    rupper(sl, hits, hero, map, clouds);
    rlower(sl, hits, hero, map, current);
    return rmiddle(sl, hits, hero, map);
}
