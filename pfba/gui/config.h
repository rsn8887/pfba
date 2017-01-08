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
#endif

class Config {

public:

    int rotation[4] {0, 90, 180, 270};

    Config(const char *cfgPath, const std::vector<RomList::Hardware> &hardwares);
    ~Config() {};

    void Load(RomList::Rom *rom = NULL);
    void Save(RomList::Rom *rom = NULL);

    int GetGuiValue(int id);
    int GetRomValue(int id);

    const char *GetRomPath(int n);

    std::vector<Option> *GetGuiOptions();
    std::vector<Option> *GetRomOptions();

    int GetOptionPos(std::vector<Option> *options, int index);

private:
    std::vector<std::string> roms_paths;
    std::vector<Option> options_gui;
    std::vector<Option> options_rom;
    char configPath[512];
    bool done = false;
};

#endif //_CONFIG_H_
