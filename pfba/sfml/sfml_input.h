//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SFML_INPUT_H
#define _SFML_INPUT_H

#include <skeleton/input.h>
#include "sfml_renderer.h"

class SFMLInput : Input {

public:
    SFMLInput(SFMLRenderer *rdr);
    virtual ~SFMLInput();

    virtual Player *Update(bool rotate = false);
    virtual int GetButton(int player);

private:
    virtual void process_axis(Input::Player& player, bool rotate = false);
    virtual void process_hat(Input::Player& player, bool rotate = false);
    virtual void process_buttons(Input::Player &player, bool rotate = false);
    virtual void process_keyboard(Input::Player& player, bool rotate = false);

    SFMLRenderer *renderer;
};

#endif //_SFML_INPUT_H
