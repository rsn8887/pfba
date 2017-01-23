//
// Created by cpasjuste on 01/12/16.
//

#include "sfml_font.h"

SFMLFont::SFMLFont(const char *path, int size) : Font(path, size) {
    this->size = size;

    if (!font.loadFromFile(path)) {
       printf("SFMLFont: load font error\n");
    }
}

SFMLFont::~SFMLFont() {
}

int SFMLFont::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    sf::Text t;
    t.setFont(font);
    t.setString(msg);
    t.setCharacterSize(size);

    return (int)t.getLocalBounds().width;
}

int SFMLFont::GetHeight(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    sf::Text t;
    t.setFont(font);
    t.setString(msg);
    t.setCharacterSize(size);

    return (int)t.getLocalBounds().height;
}
