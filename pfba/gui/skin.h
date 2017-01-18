//
// Created by cpasjuste on 23/11/16.
//

#ifndef _SKIN_H_
#define _SKIN_H_

#include <skeleton/renderer.h>

class Skin {

public:

    struct Button {
        std::string name;
        int id = -1;
        Texture *texture = NULL;
        Button(int i, const std::string &n) {
            id = i;
            name = n;
        }
    };

    Skin(Renderer *renderer, char *skinPath, std::vector<Button> btns);
    ~Skin();

    Button *GetButton(int id);

    Texture *tex_bg;
    Texture *tex_title;
    Font *font;
    std::vector<Button> buttons;
};


#endif //_SKIN_H
