//
// Created by cpasjuste on 21/11/16.
//

#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL2/SDL.h>

#include "skeleton/renderer.h"

#include "sdl2/sdl2_font.h"
#include "sdl2/sdl2_texture.h"
#include "sdl2/sdl2_fontcache.h"

class SDL2Renderer : Renderer {

public:
    SDL2Renderer(int w, int h);
    virtual ~SDL2Renderer();

    virtual Font *LoadFont(const char *path, int size);
    void DrawFont(Font *font, int x, int y, const char *fmt, ...);

    virtual Texture *CreateTexture(int w, int h);
    virtual Texture *LoadTexture(const char *file);
    virtual void DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation);
    virtual int LockTexture(Texture * texture, const Rect &rect, void **pixels, int *pitch);
    virtual void UnlockTexture(Texture * texture);

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);
    virtual void DrawRect(const Rect &rect, const Color &color, bool fill = true);

    virtual void Clip(const Rect &rect);

    virtual const Rect GetWindowSize();

    virtual void Clear();
    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int shader);

private:
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
};

#endif //SDL2_RENDERER_H
