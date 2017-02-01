//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_TEXTURE_H_
#define _CTR_TEXTURE_H_

#include <skeleton/texture.h>
#include <sf2d.h>

class CTRTexture : Texture {

public:
    CTRTexture(const char *path);

    CTRTexture(int w, int h);

    ~CTRTexture();

    void SetFiltering(int filter);
    void Tile();

    sf2d_texture *tex = NULL;
    u8 *pixels = NULL;
};

#endif //_CTR_TEXTURE_H_
