//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_FONT_H_
#define _CTR_FONT_H_

#include <skeleton/font.h>
#include <skeleton/renderer.h>
#include <sftd.h>

class CTRFont : Font {

public:
    CTRFont(const char *path, int size);

    ~CTRFont();

    int GetWidth(const char *fmt, ...);

    int GetHeight(const char *fmt, ...);

    sftd_font *font = NULL;
};

#endif //_CTR_FONT_H_
