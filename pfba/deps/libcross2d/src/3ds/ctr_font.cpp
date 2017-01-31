//
// Created by cpasjuste on 01/12/16.
//

#include <cstdio>
#include <cstdarg>
#include "ctr_font.h"

CTRFont::CTRFont(const char *path, int size) : Font(path, size) {
    this->size = size;
    font = sftd_load_font_file(path);
    if (font == NULL) {
        printf("error: could not load custom font: %s \n", path);
    }
}

CTRFont::~CTRFont() {
    if (font != NULL) {
        sftd_free_font(font);
        font = NULL;
    }
}

int CTRFont::GetWidth(const char *fmt, ...) {

    if (font == NULL) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    uint s = (uint) ((float) size * scaling);
    return sftd_get_text_width(font, s, msg);
}

int CTRFont::GetHeight(const char *fmt, ...) {

    if (font == NULL) {
        return 0;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    int h, w;
    uint s = (unsigned int) ((float) size * scaling);
    sftd_calc_bounding_box(&w, &h, font, s, 300, msg);
    return h;
}
