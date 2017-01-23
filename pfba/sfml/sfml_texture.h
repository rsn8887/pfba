//
// Created by cpasjuste on 01/12/16.
//

#ifndef _SFML_TEXTURE_H_
#define _SFML_TEXTURE_H_

#include "skeleton/renderer.h"
#include "sfml_renderer.h"

class SFMLTexture : Texture {

public:
    SFMLTexture(const char *path);
    SFMLTexture(int w, int h);
    void SetFiltering(int filter);
    ~SFMLTexture();

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Uint8* pixels = NULL;
};

#endif //_SFML_TEXTURE_H_
