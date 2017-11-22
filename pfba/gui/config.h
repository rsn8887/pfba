//
// Created by cpasjuste on 05/12/16.
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "libconfig.h"

#include "gui.h"

class Gui;

#include "option.h"

#ifdef __PSP2__
#define KEY_JOY_UP_DEFAULT      8
#define KEY_JOY_DOWN_DEFAULT    6
#define KEY_JOY_LEFT_DEFAULT    7
#define KEY_JOY_RIGHT_DEFAULT   9
#define KEY_JOY_FIRE1_DEFAULT   2
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   3
#define KEY_JOY_FIRE4_DEFAULT   0
#define KEY_JOY_FIRE5_DEFAULT   4
#define KEY_JOY_FIRE6_DEFAULT   5
#define KEY_JOY_COIN1_DEFAULT   10
#define KEY_JOY_START1_DEFAULT  11
#define KEY_JOY_MENU1_DEFAULT   11
#define KEY_JOY_MENU2_DEFAULT   10
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         2
#define KEY_JOY_AXIS_RY         3
#elif __3DS__
// from <3ds/services/hid.h>
#define KEY_JOY_UP_DEFAULT      6
#define KEY_JOY_DOWN_DEFAULT    7
#define KEY_JOY_LEFT_DEFAULT    5
#define KEY_JOY_RIGHT_DEFAULT   4
#define KEY_JOY_FIRE1_DEFAULT   0
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   10
#define KEY_JOY_FIRE4_DEFAULT   11
#define KEY_JOY_FIRE5_DEFAULT   9
#define KEY_JOY_FIRE6_DEFAULT   8
#define KEY_JOY_COIN1_DEFAULT   2
#define KEY_JOY_START1_DEFAULT  3
#define KEY_JOY_MENU1_DEFAULT   3
#define KEY_JOY_MENU2_DEFAULT   2
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         0
#define KEY_JOY_AXIS_RX         0
#define KEY_JOY_AXIS_RY         0
#else
#define KEY_JOY_UP_DEFAULT      -1  // use hat
#define KEY_JOY_DOWN_DEFAULT    -1  // use hat
#define KEY_JOY_LEFT_DEFAULT    -1  // use hat
#define KEY_JOY_RIGHT_DEFAULT   -1  // use hat
#define KEY_JOY_FIRE1_DEFAULT   0
#define KEY_JOY_FIRE2_DEFAULT   1
#define KEY_JOY_FIRE3_DEFAULT   2
#define KEY_JOY_FIRE4_DEFAULT   3
#define KEY_JOY_FIRE5_DEFAULT   4
#define KEY_JOY_FIRE6_DEFAULT   5
#define KEY_JOY_COIN1_DEFAULT   6
#define KEY_JOY_START1_DEFAULT  7
#define KEY_JOY_MENU1_DEFAULT   7
#define KEY_JOY_MENU2_DEFAULT   6
#define KEY_JOY_AXIS_LX         0
#define KEY_JOY_AXIS_LY         1
#define KEY_JOY_AXIS_RX         4
#define KEY_JOY_AXIS_RY         5
#endif

class Config {

public:

    Config(const std::string &cfgPath, Renderer *renderer);

    ~Config() {};

    void Load(RomList::Rom *rom = NULL);

    void Save(RomList::Rom *rom = NULL);

    int GetGuiValue(int id);

    int GetRomValue(int id);

    const char *GetRomPath(int n);

    std::vector<std::string> GetRomPaths();

    std::vector<Option> *GetGuiOptions();

    std::vector<Option> *GetRomOptions();

    int GetOptionPos(std::vector<Option> *options, int index);

    Option *GetOption(std::vector<Option> *options, int index);

    int *GetGuiPlayerInputKeys(int player);

    int *GetGuiPlayerInputButtons(int player);

    int *GetRomPlayerInputKeys(int player);

    int *GetRomPlayerInputButtons(int player);

    std::vector<RomList::Hardware> hardwareList;

private:
    std::vector<std::string> roms_paths;
    std::vector<Option> options_gui;
    std::vector<Option> options_rom;
    std::string configPath;
    bool done = false;

    int keyboard_keys[KEY_COUNT];
    int joystick_keys[KEY_COUNT];
};

#endif //_CONFIG_H_
