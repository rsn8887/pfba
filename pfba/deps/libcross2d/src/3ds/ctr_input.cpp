//
// Created by cpasjuste on 11/01/17.
//

#include <3ds.h>
#include "ctr_input.h"

static int key_id[KEY_COUNT]{
        Input::Key::KEY_UP,
        Input::Key::KEY_DOWN,
        Input::Key::KEY_LEFT,
        Input::Key::KEY_RIGHT,
        Input::Key::KEY_COIN,
        Input::Key::KEY_START,
        Input::Key::KEY_FIRE1,
        Input::Key::KEY_FIRE2,
        Input::Key::KEY_FIRE3,
        Input::Key::KEY_FIRE4,
        Input::Key::KEY_FIRE5,
        Input::Key::KEY_FIRE6
};

CTRInput::CTRInput() {

    /*
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        printf("SDL2Input: SDL_INIT_JOYSTICK\n");
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    int joystick_count = SDL_NumJoysticks();
    if (joystick_count > 4) {
        joystick_count = 4;
    }
    printf("%d Joystick(s) Found\n", joystick_count);

    if (joystick_count > 0) {
        for (int i = 0; i < joystick_count; i++) {
            printf("Joystick: %i\n", i);
            players[i].data = SDL_JoystickOpen(i);
            players[i].id = i;
            players[i].enabled = true;
            printf("Name: %s\n", SDL_JoystickName((SDL_Joystick *) players[i].data));
            printf("Hats %d\n", SDL_JoystickNumHats((SDL_Joystick *) players[i].data));
            printf("Buttons %d\n", SDL_JoystickNumButtons((SDL_Joystick *) players[i].data));
            printf("Axis %d\n", SDL_JoystickNumAxes((SDL_Joystick *) players[i].data));
        }
    } else {
        // allow keyboard mapping to player1
        players[0].enabled = true;
    }
    */

    for (int i = 0; i < PLAYER_COUNT; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

CTRInput::~CTRInput() {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].enabled = false;
    }
}

int CTRInput::GetButton(int player) {

    /*
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            return event.jbutton.button;
        }
    }
    */
    return -1;
}

Input::Player *CTRInput::Update(bool rotate) {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].state = 0;
    }


    if (!aptMainLoop()) {
        players[0].state |= EV_QUIT;
        return players;
    }

    circlePosition circle;
    hidScanInput();
    hidCircleRead(&circle);

    // 3ds needs screen refresh/swap every frames ?
    if (players[0].state <= 0) {
        players[0].state = EV_REFRESH;
    }

    /*
    for (int i = 0; i < PLAYER_COUNT; i++) {

        if (!players[i].enabled) {
            continue;
        }

        // hat
        process_hat(players[i], rotate);

        // sticks
        process_axis(players[i], rotate);

        // buttons
        process_buttons(players[i], rotate);
    }

    // keyboard
    process_keyboard(players[0], rotate);
    */

    return players;
}

void CTRInput::process_axis(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }


}

void CTRInput::process_hat(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }


}

void CTRInput::process_buttons(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];

    }
}
