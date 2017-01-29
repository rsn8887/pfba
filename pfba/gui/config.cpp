//
// Created by cpasjuste on 05/12/16.
//

#include <libconfig.h>
#include <string>
#include <burner.h>
#include "config.h"

Config::Config(const std::string &cfgPath, Renderer *renderer) {

    configPath = cfgPath;

    options_gui.clear();

    // gui config
    roms_paths.clear();
#ifdef __PSP2__
    roms_paths.push_back("ux0:/data/pfba/roms/");
#else
    roms_paths.push_back("./roms/");
#endif
    roms_paths.push_back("");
    roms_paths.push_back("");
    roms_paths.push_back("");

    // build hardware list configuration
    hardwareList.push_back({HARDWARE_PREFIX_ALL, "All"});
    hardwareList.push_back({HARDWARE_PREFIX_SNK, "Neo Geo"});
    hardwareList.push_back({HARDWARE_PREFIX_CAPCOM, "CPS-1"});
    hardwareList.push_back({HARDWARE_PREFIX_CPS2, "CPS-2"});
    hardwareList.push_back({HARDWARE_PREFIX_CPS3, "CPS-3"});
    hardwareList.push_back({HARDWARE_PREFIX_IGS_PGM, "PGM"});
    hardwareList.push_back({HARDWARE_PREFIX_PSIKYO, "Psikyo"});
    hardwareList.push_back({HARDWARE_PREFIX_CAVE, "Cave"});
    hardwareList.push_back({HARDWARE_PREFIX_SEGA, "Sega"});
    hardwareList.push_back({HARDWARE_PREFIX_CAPCOM_MISC, "Capcom (Other)"});
    hardwareList.push_back({HARDWARE_PREFIX_DATAEAST, "Data East"});
    hardwareList.push_back({HARDWARE_PREFIX_GALAXIAN, "Galaxian"});
    hardwareList.push_back({HARDWARE_PREFIX_IREM, "Irem"});
    hardwareList.push_back({HARDWARE_PREFIX_KANEKO, "Kaneko"});
    hardwareList.push_back({HARDWARE_PREFIX_KONAMI, "Konami"});
    hardwareList.push_back({HARDWARE_PREFIX_MISC_PRE90S, "Misc (pre 90s)"});
    hardwareList.push_back({HARDWARE_PREFIX_MISC_POST90S, "Misc (post 90s)"});
    hardwareList.push_back({HARDWARE_PREFIX_PACMAN, "Pacman"});
    hardwareList.push_back({HARDWARE_PREFIX_SETA, "Seta"});
    hardwareList.push_back({HARDWARE_PREFIX_TAITO, "Taito"});
    hardwareList.push_back({HARDWARE_PREFIX_TECHNOS, "Technos"});
    hardwareList.push_back({HARDWARE_PREFIX_TOAPLAN, "Toaplan"});
    hardwareList.push_back({HARDWARE_PREFIX_SEGA_MEGADRIVE, "Sega MegaDrive"});
    hardwareList.push_back({HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem"});
    hardwareList.push_back({HARDWARE_PREFIX_SEGA_SG1000, "Sega SG1000"});
    hardwareList.push_back({HARDWARE_PREFIX_SEGA_GAME_GEAR, "Sega GameGear"});
    hardwareList.push_back({HARDWARE_PREFIX_PCENGINE, "PC-Engine"});
    hardwareList.push_back({HARDWARE_PREFIX_COLECO, "Coleco"});
    hardwareList.push_back({HARDWARE_PREFIX_MIDWAY, "Midway"});
    hardwareList.push_back({HARDWARE_PREFIX_MSX, "MSX"});

    std::vector<std::string> hardware_names;
    for (int i = 0; i < hardwareList.size(); i++) {
        hardware_names.push_back(hardwareList[i].name);
    }

    ////////////////////////////////////////////////////////////
    // options needs to be in sync/order with "Option::Index"
    ////////////////////////////////////////////////////////////

    // main/gui config
    options_gui.push_back(Option("MAIN", {"MAIN"}, 0, Option::Index::MENU_MAIN, Option::Type::MENU));
    options_gui.push_back(Option("SHOW_ALL", {"WORKING", "ALL"}, 1, Option::Index::GUI_SHOW_ALL));
    options_gui.push_back(Option("SHOW_CLONES", {"NO", "YES"}, 0, Option::Index::GUI_SHOW_CLONES));
    options_gui.push_back(Option("SHOW_HARDWARE", hardware_names, 0, Option::Index::GUI_SHOW_HARDWARE));
    options_gui.push_back(Option("FULLSCREEN", {"NO", "YES"}, 1, Option::Index::GUI_FULLSCREEN, Option::Type::HIDDEN));

    // default rom config
    options_gui.push_back(Option("ROM", {"ROM"}, 0, Option::Index::MENU_ROM_OPTIONS, Option::Type::MENU));
    options_gui.push_back(Option("SCALING", {"NONE", "2X", "FIT", "FIT 4:3", "FIT 3:4", "FULL"}, 1, Option::Index::ROM_SCALING));
    options_gui.push_back(
            Option("FILTER", {"POINT", "LINEAR"}, 0, Option::Index::ROM_FILTER));
    options_gui.push_back(
            Option("SHADER", renderer->shaders->GetNames(), 0, Option::Index::ROM_SHADER));
    options_gui.push_back(Option("ROTATION", {"OFF", "ON"}, 0, Option::Index::ROM_ROTATION));
    options_gui.push_back(Option("SHOW_FPS", {"NO", "YES"}, 0, Option::Index::ROM_SHOW_FPS));
    options_gui.push_back(Option("FRAMESKIP", {"OFF", "ON"}, 0, Option::Index::ROM_FRAMESKIP));
    //options_gui.push_back(Option("M68K", {"ASM", "C"}, 0, Option::Index::ROM_M68K));
    options_gui.push_back(Option("AUDIO_ENABLED", {"OFF", "ON"}, 1, Option::Index::ROM_AUDIO_ENABLED));

    // joystick
    options_gui.push_back(Option("JOYPAD", {"JOYPAD"}, 0, Option::Index::MENU_JOYPAD, Option::Type::MENU));
    options_gui.push_back(
            Option("JOY_UP", {"-1"}, KEY_JOY_UP_DEFAULT, Option::Index::JOY_UP, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_DOWN", {"-1"}, KEY_JOY_DOWN_DEFAULT, Option::Index::JOY_DOWN, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_LEFT", {"-1"}, KEY_JOY_LEFT_DEFAULT, Option::Index::JOY_LEFT, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_RIGHT", {"-1"}, KEY_JOY_RIGHT_DEFAULT, Option::Index::JOY_RIGHT, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE1", {"0"}, KEY_JOY_FIRE1_DEFAULT, Option::Index::JOY_FIRE1, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE2", {"1"}, KEY_JOY_FIRE2_DEFAULT, Option::Index::JOY_FIRE2, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE3", {"2"}, KEY_JOY_FIRE3_DEFAULT, Option::Index::JOY_FIRE3, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE4", {"3"}, KEY_JOY_FIRE4_DEFAULT, Option::Index::JOY_FIRE4, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE5", {"4"}, KEY_JOY_FIRE5_DEFAULT, Option::Index::JOY_FIRE5, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_FIRE6", {"5"}, KEY_JOY_FIRE6_DEFAULT, Option::Index::JOY_FIRE6, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_COIN1", {"6"}, KEY_JOY_COIN1_DEFAULT, Option::Index::JOY_COIN1, Option::Type::INPUT));
    options_gui.push_back(
            Option("JOY_START1", {"7"}, KEY_JOY_START1_DEFAULT, Option::Index::JOY_START1, Option::Type::INPUT));
    // TODO: add gui option for axis in option menu
    options_gui.push_back(
            Option("JOY_AXIS_LX", {"0"}, KEY_JOY_AXIS_LX, Option::Index::JOY_AXIS_LX, Option::Type::HIDDEN));
    options_gui.push_back(
            Option("JOY_AXIS_LY", {"1"}, KEY_JOY_AXIS_LY, Option::Index::JOY_AXIS_LY, Option::Type::HIDDEN));
    options_gui.push_back(
            Option("JOY_AXIS_RX", {"2"}, KEY_JOY_AXIS_RX, Option::Index::JOY_AXIS_RX, Option::Type::HIDDEN));
    options_gui.push_back(
            Option("JOY_AXIS_RY", {"3"}, KEY_JOY_AXIS_RY, Option::Index::JOY_AXIS_RY, Option::Type::HIDDEN));
    options_gui.push_back(Option("JOY_DEADZONE",
                                 {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                  "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3,
                                 Option::Index::JOY_DEADZONE, Option::Type::INTEGER));

    // keyboard
    options_gui.push_back(Option("KEYBOARD", {"KEYBOARD"}, 0, Option::Index::MENU_KEYBOARD, Option::Type::MENU));
    options_gui.push_back(Option("KEY_UP", {"82"}, 82, Option::Index::KEY_UP, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_DOWN", {"81"}, 81, Option::Index::KEY_DOWN, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_LEFT", {"80"}, 80, Option::Index::KEY_LEFT, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_RIGHT", {"79"}, 79, Option::Index::KEY_RIGHT, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE1", {"89"}, 89, Option::Index::KEY_FIRE1, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE2", {"90"}, 90, Option::Index::KEY_FIRE2, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE3", {"91"}, 91, Option::Index::KEY_FIRE3, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE4", {"92"}, 92, Option::Index::KEY_FIRE4, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE5", {"93"}, 93, Option::Index::KEY_FIRE5, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_FIRE6", {"94"}, 94, Option::Index::KEY_FIRE6, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_COIN1", {"41"}, 41, Option::Index::KEY_COIN1, Option::Type::INPUT));
    options_gui.push_back(Option("KEY_START1", {"40"}, 40, Option::Index::KEY_START1, Option::Type::INPUT));

    //
    options_gui.push_back(Option("END", {"END"}, 0, Option::Index::END, Option::Type::MENU));

    // set default rom options
    options_rom.clear();
    for (int i = Option::Index::MENU_ROM_OPTIONS; i < Option::Index::END; i++) {
        options_rom.push_back(options_gui[i]);
    }

    Load();
}

void Config::Load(RomList::Rom *rom) {

    config_t cfg;
    config_init(&cfg);

    bool isRomCfg = rom != NULL;
    std::vector<Option> *options = isRomCfg ? &options_rom : &options_gui;
    std::string path = configPath;
    if (isRomCfg) {
        path = szAppConfigPath;
        path += "/";
        path += rom->zip;
        path += ".cfg";
    }

    if (config_read_file(&cfg, path.c_str())) {

        //printf("###########################\n");
        //printf("CFG FOUND: %s\n", path.c_str());

        config_setting_t *settings_root = config_lookup(&cfg, "FBA_CONFIG");
        if (settings_root) {

            config_setting_t *settings = NULL;

            if (!isRomCfg) {
                settings = config_setting_lookup(settings_root, "ROMS_PATHS");
                if (settings) {
                    for (int i = 0; i < roms_paths.size(); i++) {
                        char p[MAX_PATH];
                        snprintf(p, MAX_PATH, "ROMS_PATH%i", i);
                        const char *value;
                        if (config_setting_lookup_string(settings, p, &value)) {
                            roms_paths[i] = value;
                            if (!roms_paths[i].empty() && roms_paths[i].back() != '/')
                                roms_paths[i] += '/';
                            printf("%s: %s\n", p, value);
                        }
                    }
                } else {
                    //printf("rom_paths setting not found\n");
                }
            }

            for (unsigned long i = 0; i < options->size(); i++) {
                if (options->at(i).type == Option::Type::MENU) {
                    settings = config_setting_lookup(settings_root, options->at(i).GetName());
                }
                if (settings) {
                    int value = 0;
                    if (config_setting_lookup_int(settings, options->at(i).GetName(), &value)) {
                        options->at(i).value = value;
                        //printf("%s: %i\n", options->at(i).GetName(), value);
                    }
                }
            }
        }
        //printf("###########################\n");
    } else {
        // no need to save default rom config
        if (!isRomCfg) {
            Save();
        }
        // set default rom options
        options_rom.clear();
        for (int i = Option::Index::MENU_ROM_OPTIONS; i < Option::Index::END; i++) {
            options_rom.push_back(options_gui[i]);
        }
    }

    config_destroy(&cfg);
}

void Config::Save(RomList::Rom *rom) {

    config_t cfg;
    config_init(&cfg);

    bool isRomCfg = rom != NULL;
    std::vector<Option> *options = isRomCfg ? &options_rom : &options_gui;
    std::string path = configPath;
    if (isRomCfg) {
        path = szAppConfigPath;
        path += "/";
        path += rom->zip;
        path += ".cfg";
    }

    // create root
    config_setting_t *setting_root = config_root_setting(&cfg);
    // create main group
    config_setting_t *setting_fba = config_setting_add(setting_root, "FBA_CONFIG", CONFIG_TYPE_GROUP);

    config_setting_t *sub_setting = NULL;

    if (!isRomCfg) {
        sub_setting = config_setting_add(setting_fba, "ROMS_PATHS", CONFIG_TYPE_GROUP);
        for (int i = 0; i < roms_paths.size(); i++) {
            char p[MAX_PATH];
            snprintf(p, MAX_PATH, "ROMS_PATH%i", i);
            config_setting_t *setting = config_setting_add(sub_setting, p, CONFIG_TYPE_STRING);
            config_setting_set_string(setting, roms_paths[i].c_str());
        }
    }

    for (unsigned long i = 0; i < options->size(); i++) {
        if (options->at(i).index == Option::Index::END) {
            continue;
        }
        if (options->at(i).type == Option::Type::MENU) {
            sub_setting = config_setting_add(setting_fba, options->at(i).GetName(), CONFIG_TYPE_GROUP);
            continue;
        }
        config_setting_t *setting = config_setting_add(sub_setting, options->at(i).GetName(), CONFIG_TYPE_INT);
        config_setting_set_int(setting, options->at(i).value);
    }

    config_write_file(&cfg, path.c_str());
    config_destroy(&cfg);

    if (!isRomCfg) {
        // set default rom options
        options_rom.clear();
        for (int i = Option::Index::MENU_ROM_OPTIONS; i < Option::Index::END; i++) {
            options_rom.push_back(options_gui[i]);
        }
    }
}

int Config::GetGuiValue(int index) {

    for (int i = 0; i < options_gui.size(); i++) {
        if (index == options_gui[i].index) {
            return options_gui[i].value;
        }
    }

    return 0;
}

int Config::GetRomValue(int index) {

    for (int i = 0; i < options_rom.size(); i++) {
        if (index == options_rom[i].index) {
            return options_rom[i].value;
        }
    }

    return 0;
}

const char *Config::GetRomPath(int n) {
    if (n >= DIRS_MAX) {
        return roms_paths[0].c_str();
    } else {
        return roms_paths[n].c_str();
    }
}

std::vector<std::string> Config::GetRomPaths() {
    std::vector<std::string> rom_paths;
    for (int i = 0; i < DIRS_MAX; i++) {
        rom_paths.push_back(GetRomPath(i));
    }
    return roms_paths;
}

std::vector<Option> *Config::GetGuiOptions() {
    return &options_gui;
}

std::vector<Option> *Config::GetRomOptions() {
    return &options_rom;
}

int Config::GetOptionPos(std::vector<Option> *options, int index) {
    for (int i = 0; i < options->size(); i++) {
        if (options->at(i).index == index) {
            return i;
        }
    }
    return 0;
}

int *Config::GetGuiPlayerInputKeys(int player) {

    // TODO: player > 0 not supported yet
    keyboard_keys[0] = GetGuiValue(Option::Index::KEY_UP);
    keyboard_keys[1] = GetGuiValue(Option::Index::KEY_DOWN);
    keyboard_keys[2] = GetGuiValue(Option::Index::KEY_LEFT);
    keyboard_keys[3] = GetGuiValue(Option::Index::KEY_RIGHT);
    keyboard_keys[4] = GetGuiValue(Option::Index::KEY_COIN1);
    keyboard_keys[5] = GetGuiValue(Option::Index::KEY_START1);
    keyboard_keys[6] = GetGuiValue(Option::Index::KEY_FIRE1);
    keyboard_keys[7] = GetGuiValue(Option::Index::KEY_FIRE2);
    keyboard_keys[8] = GetGuiValue(Option::Index::KEY_FIRE3);
    keyboard_keys[9] = GetGuiValue(Option::Index::KEY_FIRE4);
    keyboard_keys[10] = GetGuiValue(Option::Index::KEY_FIRE5);
    keyboard_keys[11] = GetGuiValue(Option::Index::KEY_FIRE6);

    return keyboard_keys;
}

int *Config::GetGuiPlayerInputButtons(int player) {

    // TODO: player > 0 not supported yet
    joystick_keys[0] = GetGuiValue(Option::Index::JOY_UP);
    joystick_keys[1] = GetGuiValue(Option::Index::JOY_DOWN);
    joystick_keys[2] = GetGuiValue(Option::Index::JOY_LEFT);
    joystick_keys[3] = GetGuiValue(Option::Index::JOY_RIGHT);
    joystick_keys[4] = GetGuiValue(Option::Index::JOY_COIN1);
    joystick_keys[5] = GetGuiValue(Option::Index::JOY_START1);
    joystick_keys[6] = GetGuiValue(Option::Index::JOY_FIRE1);
    joystick_keys[7] = GetGuiValue(Option::Index::JOY_FIRE2);
    joystick_keys[8] = GetGuiValue(Option::Index::JOY_FIRE3);
    joystick_keys[9] = GetGuiValue(Option::Index::JOY_FIRE4);
    joystick_keys[10] = GetGuiValue(Option::Index::JOY_FIRE5);
    joystick_keys[11] = GetGuiValue(Option::Index::JOY_FIRE6);

    return joystick_keys;
}

int *Config::GetRomPlayerInputKeys(int player) {

    // TODO: player > 0 not supported yet
    keyboard_keys[0] = GetRomValue(Option::Index::KEY_UP);
    keyboard_keys[1] = GetRomValue(Option::Index::KEY_DOWN);
    keyboard_keys[2] = GetRomValue(Option::Index::KEY_LEFT);
    keyboard_keys[3] = GetRomValue(Option::Index::KEY_RIGHT);
    keyboard_keys[4] = GetRomValue(Option::Index::KEY_COIN1);
    keyboard_keys[5] = GetRomValue(Option::Index::KEY_START1);
    keyboard_keys[6] = GetRomValue(Option::Index::KEY_FIRE1);
    keyboard_keys[7] = GetRomValue(Option::Index::KEY_FIRE2);
    keyboard_keys[8] = GetRomValue(Option::Index::KEY_FIRE3);
    keyboard_keys[9] = GetRomValue(Option::Index::KEY_FIRE4);
    keyboard_keys[10] = GetRomValue(Option::Index::KEY_FIRE5);
    keyboard_keys[11] = GetRomValue(Option::Index::KEY_FIRE6);

    return keyboard_keys;
}

int *Config::GetRomPlayerInputButtons(int player) {

    // TODO: player > 0 not supported yet
    joystick_keys[0] = GetRomValue(Option::Index::JOY_UP);
    joystick_keys[1] = GetRomValue(Option::Index::JOY_DOWN);
    joystick_keys[2] = GetRomValue(Option::Index::JOY_LEFT);
    joystick_keys[3] = GetRomValue(Option::Index::JOY_RIGHT);
    joystick_keys[4] = GetRomValue(Option::Index::JOY_COIN1);
    joystick_keys[5] = GetRomValue(Option::Index::JOY_START1);
    joystick_keys[6] = GetRomValue(Option::Index::JOY_FIRE1);
    joystick_keys[7] = GetRomValue(Option::Index::JOY_FIRE2);
    joystick_keys[8] = GetRomValue(Option::Index::JOY_FIRE3);
    joystick_keys[9] = GetRomValue(Option::Index::JOY_FIRE4);
    joystick_keys[10] = GetRomValue(Option::Index::JOY_FIRE5);
    joystick_keys[11] = GetRomValue(Option::Index::JOY_FIRE6);

    return joystick_keys;
}
