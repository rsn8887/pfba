//
// Created by cpasjuste on 05/12/16.
//

#ifndef _OPTION_H_
#define _OPTION_H_

#include <string>
#include <vector>

#define OPTION_RETURN (Option::Index)100
#define OPTION_STATES (Option::Index)101
#define OPTION_EXIT (Option::Index)102

class Option {

public:

    enum Type {
        MENU = 0,
        INTEGER,
        INPUT,
        HIDDEN
    };

    enum Index {
        MENU_MAIN = 0,
        GUI_SHOW_ALL,
        GUI_SHOW_CLONES,
        GUI_SHOW_HARDWARE,
        GUI_FULLSCREEN,
        MENU_ROM_OPTIONS,
        ROM_SCALING,
        ROM_FILTER,
        ROM_SHADER,
        ROM_ROTATION,
        ROM_SHOW_FPS,
        //ROM_M68K,
        ROM_FRAMESKIP,
        ROM_FORCE_60HZ,
        ROM_AUDIO_FREQ,
        //ROM_AUDIO_INTERPOLATION,
        //ROM_AUDIO_FMINTERPOLATION,
        MENU_JOYPAD,
        JOY_UP,
        JOY_DOWN,
        JOY_LEFT,
        JOY_RIGHT,
        JOY_FIRE1,
        JOY_FIRE2,
        JOY_FIRE3,
        JOY_FIRE4,
        JOY_FIRE5,
        JOY_FIRE6,
        JOY_COIN1,
        JOY_START1,
        JOY_AXIS_LX,
        JOY_AXIS_LY,
        JOY_AXIS_RX,
        JOY_AXIS_RY,
        JOY_DEADZONE,
        MENU_KEYBOARD,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_FIRE1,
        KEY_FIRE2,
        KEY_FIRE3,
        KEY_FIRE4,
        KEY_FIRE5,
        KEY_FIRE6,
        KEY_COIN1,
        KEY_START1,
        END
    };

    Option(const std::string &text, const std::vector<std::string> &options, int defaultValue, Index idx, Type type = INTEGER);

    Type type = INTEGER;
    int index = 0;
    int value = 0;
    const char *GetName();
    void SetName(const std::string &name);
    const char *GetValue();
    void Next();
    void Prev();

private:
    std::string text;
    std::vector<std::string> options;
};

#endif //_OPTION_H_
