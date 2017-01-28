//
// Created by cpasjuste on 01/12/16.
//

#include <cstdio>
#include <cstdarg>
#include "psp2_font.h"

PSP2Font::PSP2Font(const char *path, int size) : Font(path, size) {
    this->size = size;
    font = vita2d_load_custom_pgf(path);
    if (font == NULL) {
        printf("error: could not load custom font: %s \n", path);
    }
}

PSP2Font::~PSP2Font() {
    if (font != NULL) {
        vita2d_free_pgf(font);
        font = NULL;
    }
}

int PSP2Font::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) vita2d_pgf_text_width(font, scaling, msg);
}

int PSP2Font::GetHeight(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    return (int) vita2d_pgf_text_height(font, scaling, msg);
}
