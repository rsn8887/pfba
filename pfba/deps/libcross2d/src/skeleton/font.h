//
// Created by cpasjuste on 01/12/16.
//

#ifndef _FONT_H_
#define _FONT_H_

#include "color.h"

class Font {

public:
    Font(const char *path, int size) {};

    virtual ~Font() {};

    virtual int GetWidth(const char *fmt, ...) {};
    virtual int GetHeight(const char *fmt, ...) {};

    Color color {255, 255, 255, 255};
    int size = 20;
    float scaling = 1;
};

#endif //_FONT_H
