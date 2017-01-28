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

int SFMLFont::GetWidth(const char *fmt, ...) {

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize((unsigned int) size);
    t.setString(msg);

    return (int) t.getLocalBounds().width + (int)(t.getLocalBounds().left/2.0f);
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
    t.setCharacterSize((unsigned int) size);
    t.setString(msg);

    return (int) ((t.getLocalBounds().height/2) * (t.getLocalBounds().height/t.getLocalBounds().top));
}
