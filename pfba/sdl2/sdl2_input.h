//
// Created by cpasjuste on 11/01/17.
//

#ifndef _SDL2_INPUT_H
#define _SDL2_INPUT_H

#include <skeleton/input.h>
#include <SDL2/SDL.h>

class SDL2Input : Input {

public:
    SDL2Input();
    virtual ~SDL2Input();

    virtual Player *Update(bool rotate = false);
    virtual int Wait(int player);
    virtual int Clear(int player);

private:
    virtual void process_axis(Input::Player& player, bool rotate = false);
    virtual void process_hat(Input::Player& player, bool rotate = false);
    virtual void process_buttons(Input::Player &player, bool rotate = false);
    virtual void process_keyboard(Input::Player& player, bool rotate = false);

};

#endif //_SDL2_INPUT_H
