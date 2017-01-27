//
// Created by cpasjuste on 01/12/16.
//

#ifndef _PSP2_TEXTURE_H_
#define _PSP2_TEXTURE_H_

#include <skeleton/texture.h>
#include "vita2d.h"

class PSP2Texture : Texture {

public:
    PSP2Texture(const char *path);

    PSP2Texture(int w, int h);

    ~PSP2Texture();

    void SetFiltering(int filter);

    vita2d_texture *tex = NULL;
};

#endif //_PSP2_TEXTURE_H_
