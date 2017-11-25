//
// Created by cpasjuste on 01/12/16.
//

#ifndef _CTR_FONT_H_
#define _CTR_FONT_H_

#include <skeleton/font.h>
#include <skeleton/renderer.h>

typedef struct {
    float position[3];
    float texcoord[2];
} textVertex_s;

class CTRFont : Font {

public:
    CTRFont(const char *path, int size);

    ~CTRFont();

    void Draw(float x, float y, float scaleX, float scaleY, bool baseline, const char *text);

    int GetWidth(const char *fmt, ...);

    int GetHeight(const char *fmt, ...);

    int textVtxArrayPos = 0;

private:
    void AddTextVertex(float vx, float vy, float tx, float ty);

    C3D_Tex *glyphSheets;
    textVertex_s *textVtxArray;
};

#endif //_CTR_FONT_H_
