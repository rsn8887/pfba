//
// Created by cpasjuste on 11/01/17.
//

#include "sdl2_input.h"

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

SDL2Input::SDL2Input() {

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

    for (int i = 0; i < PLAYER_COUNT; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

SDL2Input::~SDL2Input() {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].enabled = false;
    }

    if (SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }
}

int SDL2Input::GetButton(int player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            return event.jbutton.button;
        }
    }
    return -1;
}

Input::Player *SDL2Input::Update(bool rotate) {

    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].state = 0;
    }

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            players[0].state |= EV_QUIT;
            return players;
        }
    }

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

    return players;
}

void SDL2Input::process_axis(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    int x[2] = {SDL_JoystickGetAxis((SDL_Joystick *) player.data, player.axis_lx),
                SDL_JoystickGetAxis((SDL_Joystick *) player.data, player.axis_rx)};

    if (x[0] > player.dead_zone || x[1] > player.dead_zone) {
        player.state |= rotate ? Input::Key::KEY_DOWN : Input::Key::KEY_RIGHT;
    } else if (x[0] < -player.dead_zone || x[1] < -player.dead_zone) {
        player.state |= rotate ? Input::Key::KEY_UP : Input::Key::KEY_LEFT;
    }

    int y[2] = {SDL_JoystickGetAxis((SDL_Joystick *) player.data, player.axis_ly),
                SDL_JoystickGetAxis((SDL_Joystick *) player.data, player.axis_ry)};

    if (y[0] > player.dead_zone || y[1] > player.dead_zone) {
        player.state |= rotate ? Input::Key::KEY_LEFT : Input::Key::KEY_DOWN;
    } else if (y[0] < -player.dead_zone || y[1] < -player.dead_zone) {
        player.state |= rotate ? Input::Key::KEY_RIGHT : Input::Key::KEY_UP;
    }
}

void SDL2Input::process_hat(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    int value = SDL_JoystickGetHat((SDL_Joystick *) player.data, 0);

    if (value == SDL_HAT_UP
        || value == SDL_HAT_LEFTUP
        || value == SDL_HAT_RIGHTUP) {
        player.state |= rotate ? Input::Key::KEY_RIGHT : Input::Key::KEY_UP;
    }
    if (value == SDL_HAT_DOWN
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_RIGHTDOWN) {
        player.state |= rotate ? Input::Key::KEY_LEFT : Input::Key::KEY_DOWN;
    }
    if (value == SDL_HAT_LEFT
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_LEFTUP) {
        player.state |= rotate ? Input::Key::KEY_UP : Input::Key::KEY_LEFT;
    }
    if (value == SDL_HAT_RIGHT
        || value == SDL_HAT_RIGHTDOWN
        || value == SDL_HAT_RIGHTUP) {
        player.state |= rotate ? Input::Key::KEY_DOWN : Input::Key::KEY_RIGHT;
    }
}

void SDL2Input::process_buttons(Input::Player &player, bool rotate) {

    if (!player.enabled || !player.data) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];

#ifdef __PSP2__
        // rotate buttons on ps vita to play in portrait mode
        if (rotate) {
            switch (mapping) {
                case 2: // PSP2_CROSS (SDL-Vita)
                    mapping = 1; // PSP2_CIRCLE (SDL-Vita)
                    break;
                case 3: // PSP2_SQUARE (SDL-Vita)
                    mapping = 2; // PSP2_CROSS (SDL-Vita)
                    break;
                case 0: // PSP2_TRIANGLE (SDL-Vita)
                    mapping = 3; // PSP2_SQUARE (SDL-Vita)
                    break;
                case 1: // PSP2_CIRCLE (SDL-Vita)
                    mapping = 0; // PSP2_TRIANGLE (SDL-Vita)
                    break;
                default:
                    break;
            }
        }
#endif

        if (SDL_JoystickGetButton((SDL_Joystick *) player.data, mapping)) {
            if (rotate && key_id[i] == Input::Key::KEY_UP) {
                player.state |= Input::Key::KEY_RIGHT;
            } else if (rotate && key_id[i] == Input::Key::KEY_DOWN) {
                player.state |= Input::Key::KEY_LEFT;
            } else if (rotate && key_id[i] == Input::Key::KEY_LEFT) {
                player.state |= Input::Key::KEY_UP;
            } else if (rotate && key_id[i] == Input::Key::KEY_RIGHT) {
                player.state |= Input::Key::KEY_DOWN;
            } else {
                player.state |= key_id[i];
            }
        }
    }
}

void SDL2Input::process_keyboard(Input::Player &player, bool rotate) {

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < KEY_COUNT; i++) {
        if (keys[keyboard.mapping[i]]) {
            if (rotate && key_id[i] == Input::Key::KEY_UP) {
                player.state |= Input::Key::KEY_RIGHT;
            } else if (rotate && key_id[i] == Input::Key::KEY_DOWN) {
                player.state |= Input::Key::KEY_LEFT;
            } else if (rotate && key_id[i] == Input::Key::KEY_LEFT) {
                player.state |= Input::Key::KEY_UP;
            } else if (rotate && key_id[i] == Input::Key::KEY_RIGHT) {
                player.state |= Input::Key::KEY_DOWN;
            } else {
                player.state |= key_id[i];
            }
        }
    }
}
