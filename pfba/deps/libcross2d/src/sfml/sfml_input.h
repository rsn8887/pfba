//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SFML_INPUT_H
#define _SFML_INPUT_H

#include "skeleton/input.h"
class SFMLRenderer;

class SFMLInput : Input {

public:
    SFMLInput(SFMLRenderer *rdr);
    virtual ~SFMLInput();

    virtual Player *Update(bool rotate = false);
    virtual int GetButton(int player);

private:
    virtual void process_axis(Player& player, bool rotate = false);
    virtual void process_hat(Player& player, bool rotate = false);
    virtual void process_buttons(Player &player, bool rotate = false);
    virtual void process_keyboard(Player& player, bool rotate = false);

    SFMLRenderer *renderer;
};

#endif //_SFML_INPUT_H
