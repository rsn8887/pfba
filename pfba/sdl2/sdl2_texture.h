//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SDL2_TEXTURE_H_
#define _SDL2_TEXTURE_H_

#include <skeleton/renderer.h>

class SDL2Texture : Texture {

public:
    SDL2Texture(SDL_Renderer *renderer, const char *path);
    SDL2Texture(SDL_Renderer *renderer, int w, int h);
    void SetFiltering(int filter);
    ~SDL2Texture();

    SDL_Texture *tex = NULL;
    SDL_Renderer *renderer = NULL;
};

#endif //_SDL2_TEXTURE_H_
