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

    virtual Player *Update(int rotate = 0);
    virtual int GetButton(int player);

private:
    virtual void process_axis(Player& player, int rotate = 0);
    virtual void process_hat(Player& player, int rotate = 0);
    virtual void process_buttons(Player &player, int rotate = 0);
    virtual void process_keyboard(Player& player, int rotate = 0);

    SFMLRenderer *renderer;
};

#endif //_SFML_INPUT_H
