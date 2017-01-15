//
// Created by cpasjuste on 11/01/17.
//

#ifndef _INPUT_H
#define _INPUT_H

#include <cstring>
#include <vector>

#define PLAYER_COUNT 4

class Input {

public:

    enum Key {
        KEY_UP = 0x0001,
        KEY_DOWN = 0x0002,
        KEY_LEFT = 0x0004,
        KEY_RIGHT = 0x0008,
        KEY_COIN = 0x0010,
        KEY_START = 0x0020,
        KEY_FIRE1 = 0x0040,
        KEY_FIRE2 = 0x0080,
        KEY_FIRE3 = 0x0100,
        KEY_FIRE4 = 0x0200,
        KEY_FIRE5 = 0x0400,
        KEY_FIRE6 = 0x0800,
        KEY_QUIT = 0x1000,
        KEY_COUNT = 12 // don't parse KEY_QUIT
    };

    struct Player {
        int mapping[KEY_COUNT];
        int axis_lx = 0;
        int axis_ly = 1;
        int axis_rx = 2;
        int axis_ry = 3;
        unsigned int state;
        int dead_zone = 8000;
        bool enabled = false;
        void *data = NULL;
    };

    // map keyboard to player 0
    struct Keyboard {
        int mapping[KEY_COUNT];
    };

    Input() {};

    virtual ~Input() {};

    virtual Player *Update(bool rotate = false) { return players; }; // to implement
    virtual int GetButton(int player) { return -1; }; // to implement

    virtual int Clear(int player);
    virtual void SetJoystickMapping(int player, int *mapping, int deadzone = 8000);
    virtual void SetKeyboardMapping(int *mapping);

    Player players[PLAYER_COUNT];
    Keyboard keyboard;
};

#endif //_INPUT_H
