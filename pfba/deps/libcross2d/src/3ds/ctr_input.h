//
// Created by cpasjuste on 11/01/17.
//

#ifndef _CTR_INPUT_H
#define _CTR_INPUT_H

#include <skeleton/input.h>

class CTRInput : Input {

public:
    CTRInput();
    virtual ~CTRInput();

    virtual Player *Update(bool rotate = false);
    virtual int GetButton(int player);

private:
    virtual void process_axis(Input::Player& player, bool rotate = false);
    virtual void process_hat(Input::Player& player, bool rotate = false);
    virtual void process_buttons(Input::Player &player, bool rotate = false);

};

#endif //_CTR_INPUT_H
