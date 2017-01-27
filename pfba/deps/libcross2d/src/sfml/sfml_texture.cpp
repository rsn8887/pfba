//
// Created by cpasjuste on 01/12/16.
//


#include "sfml_texture.h"

SFMLTexture::SFMLTexture(const char *path) : Texture(path) {

    if (texture.loadFromFile(path)) {
        width = texture.getSize().x;
        height = texture.getSize().y;
        sprite.setTexture(texture);
        pixels = new sf::Uint8[width * height * 2];
    } else {
        printf("Couldn't create texture: %s\n", path);
    }
}

SFMLTexture::SFMLTexture(int w, int h) : Texture(w, h) {

    if(texture.create(w, h)) {
        width = w;
        height = h;
        pixels = new sf::Uint8[width * height * 2];
        sprite.setTexture(texture);
    } else {
        printf("Couldn't create texture\n");
    }
}

void SFMLTexture::SetFiltering(int filter) {
    texture.setSmooth((bool)filter);
}

SFMLTexture::~SFMLTexture() {
    if(pixels) {
        delete(pixels);
    }
}
