//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_TEXTURE_H_
#define _CTR_TEXTURE_H_

#include <skeleton/texture.h>
#include <3ds.h>
#include <citro3d.h>

class CTRTexture : Texture {

public:
    CTRTexture(const char *path);

    CTRTexture(int w, int h, GPU_TEXCOLOR format = GPU_RGB565);

    ~CTRTexture();

    void SetFiltering(int filter);

    void Tile();
    void TileSoft();

    C3D_Tex tex;
    u8 *pixels = NULL;
    GPU_TEXCOLOR fmt = GPU_RGB565;
    int bpp = 4;
    int pixels_size = 0;
};

#endif //_CTR_TEXTURE_H_
