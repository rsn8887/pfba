//
// Created by cpasjuste on 01/12/16.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sdl2_texture.h"

SDL2Texture::SDL2Texture(SDL_Renderer *renderer, const char *path) : Texture(path) {

    SDL_Surface *temp;

    temp = IMG_Load(path);
    if (temp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load %s: %s", path, SDL_GetError());
        return;
    }

    tex = SDL_CreateTextureFromSurface(renderer, temp);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(temp);

    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
}

SDL2Texture::SDL2Texture(SDL_Renderer *renderer, int w, int h) : Texture(w, h) {
    tex = SDL_CreateTexture(renderer,
                            SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
    }

    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
}

SDL2Texture::~SDL2Texture() {
    if(tex != NULL) {
        SDL_DestroyTexture(tex);
        tex = NULL;
    }
}
