//
// Created by cpasjuste on 01/12/16.
//

#include <SDL2/SDL_image.h>
#include "sdl2_texture.h"

SDL2Texture::SDL2Texture(SDL_Renderer *renderer, const char *path) : Texture(path) {

    this->renderer = renderer;

    SDL_Surface *temp = IMG_Load(path);
    if (temp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s", path, SDL_GetError());
        return;
    }

    tex = SDL_CreateTextureFromSurface(renderer, temp);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    } else {
        SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    }

    SDL_FreeSurface(temp);
}

SDL2Texture::SDL2Texture(SDL_Renderer *renderer, int w, int h) : Texture(w, h) {

    this->renderer = renderer;

    tex = SDL_CreateTexture(renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    } else {
        SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    }
}

void SDL2Texture::SetFiltering(int filter) {
    char f[16];
    snprintf(f, 16, "%i\n", filter);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, f);
    // SDL2 only set filtering on texture creation :/
    if(tex != NULL) {
        SDL_DestroyTexture(tex);
        tex = NULL;
    }
    tex = SDL_CreateTexture(renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    }
}

SDL2Texture::~SDL2Texture() {
    if(tex != NULL) {
        SDL_DestroyTexture(tex);
        tex = NULL;
    }
}
