//
// Created by cpasjuste on 05/12/16.
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "libconfig.h"

#include "gui.h"

class Gui;

#include "option.h"

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
