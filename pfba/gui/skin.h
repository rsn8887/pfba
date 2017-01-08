//
// Created by cpasjuste on 23/11/16.
//

#ifndef _SKIN_H_
#define _SKIN_H_

#include <skeleton/renderer.h>

class Skin {

public:
    Skin(char *skinPath, Renderer *renderer);

    ~Skin();

    Texture *tex_bg;
    Texture *tex_title;
    Font *font;
};


#endif //_SKIN_H
