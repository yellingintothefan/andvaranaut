#include "Font.h"

#include "util.h"

Font xfzero()
{
    static Font font;
    return font;
}

Font xfbuild(const char* const path, const int size, const uint32_t color, const int outline)
{
    if(!TTF_WasInit())
        TTF_Init();

    Font f = xfzero();
    f.ttf = TTF_OpenFont(path, size);
    if(f.ttf == NULL)
        xbomb("Could not open %s\n", path);

    // Inside color of font.
    f.color.r = (color >> 0x10) & 0xFF;
    f.color.g = (color >> 0x08) & 0xFF;
    f.color.b = (color >> 0x00) & 0xFF;

    // Font Outlining.
    TTF_SetFontOutline(f.ttf, outline);
    return f;
}

static SDL_Texture* tget(const Font f, SDL_Renderer* const rend, const int alpha, const char* text)
{
    SDL_Surface* const surface = TTF_RenderText_Solid(f.ttf, text, f.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_SetTextureAlphaMod(texture, alpha < 0 ? 0 : alpha);
    SDL_FreeSurface(surface);
    return texture;
}

void xfwrt(const Font fill, const Font line, SDL_Renderer* const rend, const int x, const int y, const char* const text, const int alpha)
{
    // A string duplicate is used else the original string is chewed up with strtok.
    char* const copy = dups(text);

    // Newlines are separated by '\n'.
    const char* const delim = "\n";
    int newline = 0;
    for(char* tok = strtok(copy, delim); tok; tok = strtok(NULL, delim))
    {
        // Get font textures.
        SDL_Texture* tfill = tget(fill, rend, alpha, tok);
        SDL_Texture* tline = tget(line, rend, alpha, tok);

        // Get font dimensions.
        int w = 0;
        int h = 0;
        TTF_SizeText(fill.ttf, tok, &w, &h);

        // Get target rect. Assume font height never changes.
        const SDL_Rect target = { x - w / 2, y - h / 2 + h * newline, w, h };

        // Copy over font textures to renderer.
        SDL_RenderCopy(rend, tfill, NULL, &target);
        SDL_RenderCopy(rend, tline, NULL, &target);

        // Cleanup font textures.
        SDL_DestroyTexture(tfill);
        SDL_DestroyTexture(tline);

        // Next newline.
        newline++;
    }

    // Cleanup.
    free(copy);
}
