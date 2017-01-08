//
// Created by cpasjuste on 23/11/16.
//

#include "skin.h"
#include "burner.h"

Skin::Skin(char *skinPath, Renderer *renderer) {

    char path[MAX_PATH];
    memset(path, 0, MAX_PATH);
    strncpy(path, skinPath, MAX_PATH);

    char str[MAX_PATH];
    memset(str, 0, MAX_PATH);
    snprintf(str, MAX_PATH, "%s/fba_bg.png", path);
    tex_bg = renderer->LoadTexture(str);

    memset(str, 0, MAX_PATH);
    snprintf(str, MAX_PATH, "%s/title.png", path);
    tex_title = renderer->LoadTexture(str);

    memset(str, 0, MAX_PATH);
#ifdef __PSP2__
    snprintf(str, MAX_PATH, "%s/default.pgf", path);
#else
    snprintf(str, MAX_PATH, "%s/default.ttf", path);
#endif

    font = renderer->LoadFont(str, 22);
}

Skin::~Skin() {
    if(tex_bg)
        delete (tex_bg);
    if(tex_title)
        delete (tex_title);
    if(font)
        delete (font);
}
