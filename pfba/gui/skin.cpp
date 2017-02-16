//
// Created by cpasjuste on 23/11/16.
//

#include "skin.h"
#include "burner.h"

Skin::Skin(Renderer *renderer, char *skinPath, int fontSize, std::vector<Button> btns) {

    char str[MAX_PATH];
    memset(str, 0, MAX_PATH);
    snprintf(str, MAX_PATH, "%s/fba_bg.png", skinPath);
    tex_bg = renderer->LoadTexture(str);

    memset(str, 0, MAX_PATH);
    snprintf(str, MAX_PATH, "%s/title.png", skinPath);
    tex_title = renderer->LoadTexture(str);

    memset(str, 0, MAX_PATH);
#ifdef __PSP2__
    snprintf(str, MAX_PATH, "%s/default-20.pgf", skinPath);
    font_small = renderer->LoadFont(str, fontSize);
    snprintf(str, MAX_PATH, "%s/default-40.pgf", skinPath);
    font_large = renderer->LoadFont(str, (int) ((float) fontSize * 1.5f));
#elif __3DS__
    snprintf(str, MAX_PATH, "%s/default.ttf", skinPath);
    font_small = renderer->LoadFont(str, fontSize);
    font_large = renderer->LoadFont(str, (int) ((float) fontSize * 1.5f));
#else
    snprintf(str, MAX_PATH, "%s/default.ttf", skinPath);
    font_small = renderer->LoadFont(str, fontSize);
    font_large = renderer->LoadFont(str, (int) ((float) fontSize * 1.5f));
#endif

    // load buttons textures
    buttons = btns;
    for (int i = 0; i < buttons.size(); i++) {
        memset(str, 0, MAX_PATH);
        snprintf(str, MAX_PATH, "%s/buttons/%i.png", skinPath, buttons[i].id);
        buttons[i].texture = renderer->LoadTexture(str);
    }
}

Skin::Button *Skin::GetButton(int id) {

    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].id == id) {
            return &buttons[i];
        }
    }
    return NULL;
}

Skin::~Skin() {
    if (tex_bg)
        delete (tex_bg);
    if (tex_title)
        delete (tex_title);
    if (font_small)
        delete (font_small);
    if (font_large)
        delete (font_large);
}
