//
// Created by cpasjuste on 01/12/16.
//

#ifndef SFML_FONT_H_
#define SFML_FONT_H_

#include <skeleton/renderer.h>
#include <skeleton/font.h>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class SFMLFont : Font {

public:
    SFMLFont(const char *path, int size);

    ~SFMLFont();

    virtual int GetWidth(const char *fmt, ...);
    virtual int GetHeight(const char *fmt, ...);

    sf::Font font;
};

#endif //_SDL2_FONT_H_
