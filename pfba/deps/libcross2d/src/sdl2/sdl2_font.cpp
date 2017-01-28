//
// Created by cpasjuste on 01/12/16.
//

#include "sdl2_font.h"

SDL2Font::SDL2Font(SDL_Renderer *renderer, const char *path, int size) : Font(path, size) {
    this->size = size;
    font = FC_CreateFont();
    if (font != NULL) {
        FC_LoadFont(font, renderer, path, (Uint32) size,
                    FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    }
}

SDL2Font::~SDL2Font() {
    if (font != NULL) {
        FC_FreeFont(font);
        font = NULL;
    }
}

int SDL2Font::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) (FC_GetWidth(font, msg) * scaling);
}

int SDL2Font::GetHeight(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) (FC_GetHeight(font, msg) * scaling);
}
