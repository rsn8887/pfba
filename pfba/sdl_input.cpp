/*
 * FinalBurn Alpha for Dingux/OpenDingux
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <SDL2/SDL.h>
#include <skeleton/audio.h>

#include "burner.h"
#include "sdl_input.h"
#include "sdl_run.h"
#include "video.h"

int joyCount = 0;
SDL_Joystick *joys[4];

unsigned int FBA_KEYPAD[4];
unsigned char ServiceRequest = 0;
unsigned char P1P2Start = 0;

// external variables
extern bool bPauseOn; // run.cpp
extern Gui *gui;
extern Video *video;
extern Audio *audio;

static void axis_process(bool invert_axis) {

    int dead_zone = 2000 +
                    gui->GetConfig()->GetRomValue(Option::Index::JOY_DEADZONE) * 2000;

    // TODO: add joy axis config to options menu
    int x[2] = {SDL_JoystickGetAxis(joys[0], JOY_AXIS_LX),
                SDL_JoystickGetAxis(joys[0], JOY_AXIS_RX)};
#ifdef __RPI__
    if (x[0] > dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_DOWN : KEYPAD_RIGHT;
    } else if (x[0] < -dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_UP : KEYPAD_LEFT;
    }

    int y[2] = {SDL_JoystickGetAxis(joys[0], JOY_AXIS_LY),
                SDL_JoystickGetAxis(joys[0], JOY_AXIS_RY)};
    if (y[0] > dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_LEFT : KEYPAD_DOWN;
    } else if (y[0] < -dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_RIGHT : KEYPAD_UP;
    }
#else
    if (x[0] > dead_zone || x[1] > dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_DOWN : KEYPAD_RIGHT;
    } else if (x[0] < -dead_zone || x[1] < -dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_UP : KEYPAD_LEFT;
    }

    int y[2] = {SDL_JoystickGetAxis(joys[0], JOY_AXIS_LY),
                SDL_JoystickGetAxis(joys[0], JOY_AXIS_RY)};
    if (y[0] > dead_zone || y[1] > dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_LEFT : KEYPAD_DOWN;
    } else if (y[0] < -dead_zone || y[1] < -dead_zone) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_RIGHT : KEYPAD_UP;
    }
#endif
}

static void hat_process(bool invert_axis) {

    int value = SDL_JoystickGetHat(joys[0], 0);

    //if (value)
    //printf("hat_process: %i\n", value);

    if (value == SDL_HAT_UP
        || value == SDL_HAT_LEFTUP
        || value == SDL_HAT_RIGHTUP) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_RIGHT : KEYPAD_UP;
    }
    if (value == SDL_HAT_DOWN
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_RIGHTDOWN) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_LEFT : KEYPAD_DOWN;
    }
    if (value == SDL_HAT_LEFT
        || value == SDL_HAT_LEFTDOWN
        || value == SDL_HAT_LEFTUP) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_UP : KEYPAD_LEFT;
    }
    if (value == SDL_HAT_RIGHT
        || value == SDL_HAT_RIGHTDOWN
        || value == SDL_HAT_RIGHTUP) {
        FBA_KEYPAD[0] |= invert_axis ? KEYPAD_DOWN : KEYPAD_RIGHT;
    }
}

static void joy_process(bool invert_axis, int mapping, int key) {

#ifdef __PSP2__
    // rotate buttons two on vita
    if (invert_axis) {
        switch (mapping) {
            case PSP2_CROSS:
                mapping = PSP2_CIRCLE;
                break;
            case PSP2_SQUARE:
                mapping = PSP2_CROSS;
                break;
            case PSP2_TRIANGLE:
                mapping = PSP2_SQUARE;
                break;
            case PSP2_CIRCLE:
                mapping = PSP2_TRIANGLE;
                break;
            default:
                break;
        }
    }
#endif

    if (SDL_JoystickGetButton(joys[0], mapping)) {
        if (invert_axis && key == KEYPAD_UP) {
            FBA_KEYPAD[0] |= KEYPAD_RIGHT;
        } else if (invert_axis && key == KEYPAD_DOWN) {
            FBA_KEYPAD[0] |= KEYPAD_LEFT;
        } else if (invert_axis && key == KEYPAD_LEFT) {
            FBA_KEYPAD[0] |= KEYPAD_UP;
        } else if (invert_axis && key == KEYPAD_RIGHT) {
            FBA_KEYPAD[0] |= KEYPAD_DOWN;
        } else {
            FBA_KEYPAD[0] |= key;
        }
    }
}

static bool keyboard_process(bool invert_axis, int mapping, int key) {

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[mapping]) {
        if (invert_axis && key == KEYPAD_UP) {
            FBA_KEYPAD[0] |= KEYPAD_RIGHT;
        } else if (invert_axis && key == KEYPAD_DOWN) {
            FBA_KEYPAD[0] |= KEYPAD_LEFT;
        } else if (invert_axis && key == KEYPAD_LEFT) {
            FBA_KEYPAD[0] |= KEYPAD_UP;
        } else if (invert_axis && key == KEYPAD_RIGHT) {
            FBA_KEYPAD[0] |= KEYPAD_DOWN;
        } else {
            FBA_KEYPAD[0] |= key;
        }
    }
}


void sdl2_input_clear() {
    while (sdl2_input_read()) {}
}

int sdl2_input_wait_button() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_JOYBUTTONUP) {
            return event.jbutton.button;
        }
    }

    return -1;
}

int sdl2_input_read() {

    ServiceRequest = 0;
    P1P2Start = 0;
    FBA_KEYPAD[0] = 0;
    FBA_KEYPAD[1] = 0;
    FBA_KEYPAD[2] = 0;
    FBA_KEYPAD[3] = 0;

    Config *cfg = gui->GetConfig();
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        /*
        if(event.type == SDL_KEYDOWN)
            printf("key: %i \n", event.key.keysym.scancode);
        else if(event.type == SDL_JOYAXISMOTION) {
            printf("joy: %i - %i\n", event.jaxis.axis, event.jaxis.value);
        }
        */
        if (event.type == SDL_QUIT) {
            FBA_KEYPAD[0] |= KEYPAD_QUIT;
            return FBA_KEYPAD[0];
        }

        // rpi always doesn't seems to have a "deadzone" for axis
        // looping SDL_PollEvent call forever
        if (event.type == SDL_JOYAXISMOTION) {
            break;
        }
    }

    int rotate = gui->GetConfig()->GetRomValue(Option::Index::ROM_ROTATION);
    bool invert_axis = (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL)
                       && !rotate && GameLooping && !bPauseOn;

    if (joyCount > 0) {
        // hat
        hat_process(invert_axis);
        // sticks
        axis_process(invert_axis);
        // buttons
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_UP), KEYPAD_UP);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_DOWN), KEYPAD_DOWN);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_LEFT), KEYPAD_LEFT);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_RIGHT), KEYPAD_RIGHT);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE1), KEYPAD_FIRE1);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE2), KEYPAD_FIRE2);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE3), KEYPAD_FIRE3);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE4), KEYPAD_FIRE4);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE5), KEYPAD_FIRE5);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_FIRE6), KEYPAD_FIRE6);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_COIN1), KEYPAD_COIN);
        joy_process(invert_axis, cfg->GetRomValue(Option::Index::JOY_START1), KEYPAD_START);
    }

    // keyboard
    keyboard_process(invert_axis, cfg->GetRomValue(Option::Index::KEY_UP), KEYPAD_UP);
    keyboard_process(invert_axis, cfg->GetRomValue(Option::Index::KEY_DOWN), KEYPAD_DOWN);
    keyboard_process(invert_axis, cfg->GetRomValue(Option::Index::KEY_LEFT), KEYPAD_LEFT);
    keyboard_process(invert_axis, cfg->GetRomValue(Option::Index::KEY_RIGHT), KEYPAD_RIGHT);
    keyboard_process(invert_axis, cfg->GetRomValue(Option::Index::KEY_FIRE1), KEYPAD_FIRE1);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_FIRE2), KEYPAD_FIRE2);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_FIRE3), KEYPAD_FIRE3);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_FIRE4), KEYPAD_FIRE4);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_FIRE5), KEYPAD_FIRE5);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_FIRE6), KEYPAD_FIRE6);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_COIN1), KEYPAD_COIN);
    keyboard_process(false, cfg->GetRomValue(Option::Index::KEY_START1), KEYPAD_START);

    if (GameLooping && !bPauseOn) {
        // process menu
        if ((FBA_KEYPAD[0] & KEYPAD_COIN)
            && (FBA_KEYPAD[0] & KEYPAD_START)) {

            bPauseOn = true;
            audio->Pause(1);

            gui->RunOptionMenu(true);

            audio->Pause(0);
            bPauseOn = false;
        } else if ((FBA_KEYPAD[0] & KEYPAD_COIN)
                   && (FBA_KEYPAD[0] & KEYPAD_FIRE5)) {

            bPauseOn = true;
            audio->Pause(1);

            gui->RunStatesMenu();

            audio->Pause(0);
            bPauseOn = false;
        }
    }

    return FBA_KEYPAD[0];
}

void sdl_input_init() {

    joyCount = (char) SDL_NumJoysticks();
    if (joyCount > 4) joyCount = 4;
    printf("%d Joystick(s) Found\n", joyCount);

    if (joyCount > 0) {
        for (int i = 0; i < joyCount; i++) {
            joys[i] = SDL_JoystickOpen(i);
            printf("Name: %s\n", SDL_JoystickName(joys[i]));
            printf("Hats %d\n", SDL_JoystickNumHats(joys[i]));
            printf("Buttons %d\n", SDL_JoystickNumButtons(joys[i]));
            printf("Axis %d\n", SDL_JoystickNumAxes(joys[i]));
        }
    }
}
