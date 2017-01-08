//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_FONT_H_
#define _SDL2_FONT_H_

#include <skeleton/renderer.h>
#include <skeleton/font.h>
#include <sdl2_fontcache.h>

class SDL2Font : Font {

public:
    SDL2Font(SDL_Renderer *renderer, const char *path, int size);

    ~SDL2Font();

    virtual int GetWidth(const char *fmt, ...);
    virtual int GetHeight(const char *fmt, ...);

    FC_Font *font = NULL;

};

#endif //_SDL2_FONT_H_
