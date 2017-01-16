/*
 * FinalBurn Alpha
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

#include <gui/gui.h>
#include <sdl2/sdl2_input.h>

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#include <psp2/psp2_utility.h>
int _newlib_heap_size_user = 192 * 1024 * 1024;
#elif __RPI__
#include <sdl2/sdl2_utility.h>
#else
#include <sdl2/sdl2_utility.h>
#endif

Renderer *renderer;
Utility *utility;
Config *config;
RomList *romList;
Gui *gui;
Input *inp;

char szAppBurnVer[16] = VERSION;
// replaces ips_manager.cpp
bool bDoIpsPatch = 0;
void IpsApplyPatches(UINT8 *base, char *rom_name) {}
// needed by cps3run.cpp and dataeast/d_backfire.cpp
void Reinitialise() {}
// needed by neo_run.cpp
void wav_exit() {}
int bRunPause;

int main(int argc, char **argv) {
#ifdef __PSP2__
#ifdef __PSP2_DEBUG__
    psp2shell_init(3333, 5);
#endif
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(222);

    // create needed directories
    sceIoMkdir("ux0:/data/pfba", 0777);
    sceIoMkdir("ux0:/data/pfba/saves", 0777);
    sceIoMkdir("ux0:/data/pfba/configs", 0777);
    sceIoMkdir("ux0:/data/pfba/hiscore", 0777);
    sceIoMkdir("ux0:/data/pfba/samples", 0777);
    sceIoMkdir("ux0:/data/pfba/previews", 0777);
    sceIoMkdir("ux0:/data/pfba/blend", 0777);
    sceIoMkdir("ux0:/data/pfba/roms", 0777);

    renderer = (Renderer *) new PSP2Renderer(960, 544);
    utility = (Utility*)new PSP2Utility();

    std::vector<RomList::Hardware> hardwares = {
            {HARDWARE_PREFIX_ALL,                "All"},
            {HARDWARE_PREFIX_SNK,                "Neo Geo"},
            {HARDWARE_PREFIX_CAPCOM,             "CPS-1"},
            {HARDWARE_PREFIX_CPS2,               "CPS-2"},
            {HARDWARE_PREFIX_CPS3,               "CPS-3"},
            {HARDWARE_PREFIX_IGS_PGM,            "PGM"},
            {HARDWARE_PREFIX_PSIKYO,             "Psikyo"},
            {HARDWARE_PREFIX_CAVE,               "Cave"},
            {HARDWARE_PREFIX_SEGA,               "Sega"},
            {HARDWARE_PREFIX_CAPCOM_MISC,        "Capcom (Other)"},
            {HARDWARE_PREFIX_DATAEAST,           "Data East"},
            {HARDWARE_PREFIX_GALAXIAN,           "Galaxian"},
            {HARDWARE_PREFIX_IREM,               "Irem"},
            {HARDWARE_PREFIX_KANEKO,             "Kaneko"},
            {HARDWARE_PREFIX_KONAMI,             "Konami"},
            {HARDWARE_PREFIX_MISC_PRE90S,        "Misc (pre 90s)"},
            {HARDWARE_PREFIX_MISC_POST90S,       "Misc (post 90s)"},
            {HARDWARE_PREFIX_PACMAN,             "Pacman"},
            {HARDWARE_PREFIX_SETA,               "Seta"},
            {HARDWARE_PREFIX_TAITO,              "Taito"},
            {HARDWARE_PREFIX_TECHNOS,            "Technos"},
            {HARDWARE_PREFIX_TOAPLAN,            "Toaplan"},
            {HARDWARE_PREFIX_SEGA_MEGADRIVE,     "Sega MegaDrive"},
            {HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem"},
            {HARDWARE_PREFIX_SEGA_SG1000,        "Sega SG1000"},
            {HARDWARE_PREFIX_SEGA_GAME_GEAR,     "Sega GameGear"},
            {HARDWARE_PREFIX_PCENGINE,           "PC-Engine"},
            {HARDWARE_PREFIX_COLECO,             "Coleco"},
            {HARDWARE_PREFIX_MIDWAY,             "Midway"},
            {HARDWARE_PREFIX_MSX,                "MSX"}
    };
#elif __RPI__
    renderer = (Renderer *) new SDL2Renderer(0, 0);
    utility = (Utility*)new SDL2Utility();

    std::vector<RomList::Hardware> hardwares = {
            {HARDWARE_PREFIX_ALL,                "All"},
            {HARDWARE_PREFIX_SNK,                "Neo Geo"},
            {HARDWARE_PREFIX_CAPCOM,             "CPS-1"},
            {HARDWARE_PREFIX_CPS2,               "CPS-2"},
            {HARDWARE_PREFIX_CPS3,               "CPS-3"},
            {HARDWARE_PREFIX_IGS_PGM,            "PGM"},
            {HARDWARE_PREFIX_PSIKYO,             "Psikyo"},
            {HARDWARE_PREFIX_CAVE,               "Cave"},
            {HARDWARE_PREFIX_SEGA,               "Sega"},
            {HARDWARE_PREFIX_CAPCOM_MISC,        "Capcom (Other)"},
            {HARDWARE_PREFIX_DATAEAST,           "Data East"},
            {HARDWARE_PREFIX_GALAXIAN,           "Galaxian"},
            {HARDWARE_PREFIX_IREM,               "Irem"},
            {HARDWARE_PREFIX_KANEKO,             "Kaneko"},
            {HARDWARE_PREFIX_KONAMI,             "Konami"},
            {HARDWARE_PREFIX_MISC_PRE90S,        "Misc (pre 90s)"},
            {HARDWARE_PREFIX_MISC_POST90S,       "Misc (post 90s)"},
            {HARDWARE_PREFIX_PACMAN,             "Pacman"},
            {HARDWARE_PREFIX_SETA,               "Seta"},
            {HARDWARE_PREFIX_TAITO,              "Taito"},
            {HARDWARE_PREFIX_TECHNOS,            "Technos"},
            {HARDWARE_PREFIX_TOAPLAN,            "Toaplan"},
            {HARDWARE_PREFIX_SEGA_MEGADRIVE,     "Sega MegaDrive"},
            {HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem"},
            {HARDWARE_PREFIX_SEGA_SG1000,        "Sega SG1000"},
            {HARDWARE_PREFIX_SEGA_GAME_GEAR,     "Sega GameGear"},
            {HARDWARE_PREFIX_PCENGINE,           "PC-Engine"},
            {HARDWARE_PREFIX_COLECO,             "Coleco"},
            {HARDWARE_PREFIX_MIDWAY,             "Midway"},
            {HARDWARE_PREFIX_MSX,                "MSX"}
    };
#else
    renderer = (Renderer *) new SDL2Renderer(960, 544);
    utility = (Utility *) new SDL2Utility();

    std::vector<RomList::Hardware> hardwares {
            {HARDWARE_PREFIX_ALL,                "All"},
            {HARDWARE_PREFIX_SNK,                "Neo Geo"},
            {HARDWARE_PREFIX_CAPCOM,             "CPS-1"},
            {HARDWARE_PREFIX_CPS2,               "CPS-2"},
            {HARDWARE_PREFIX_CPS3,               "CPS-3"},
            {HARDWARE_PREFIX_IGS_PGM,            "PGM"},
            {HARDWARE_PREFIX_PSIKYO,             "Psikyo"},
            {HARDWARE_PREFIX_CAVE,               "Cave"},
            {HARDWARE_PREFIX_SEGA,               "Sega"},
            {HARDWARE_PREFIX_CAPCOM_MISC,        "Capcom (Other)"},
            {HARDWARE_PREFIX_DATAEAST,           "Data East"},
            {HARDWARE_PREFIX_GALAXIAN,           "Galaxian"},
            {HARDWARE_PREFIX_IREM,               "Irem"},
            {HARDWARE_PREFIX_KANEKO,             "Kaneko"},
            {HARDWARE_PREFIX_KONAMI,             "Konami"},
            {HARDWARE_PREFIX_MISC_PRE90S,        "Misc (pre 90s)"},
            {HARDWARE_PREFIX_MISC_POST90S,       "Misc (post 90s)"},
            {HARDWARE_PREFIX_PACMAN,             "Pacman"},
            {HARDWARE_PREFIX_SETA,               "Seta"},
            {HARDWARE_PREFIX_TAITO,              "Taito"},
            {HARDWARE_PREFIX_TECHNOS,            "Technos"},
            {HARDWARE_PREFIX_TOAPLAN,            "Toaplan"},
            {HARDWARE_PREFIX_SEGA_MEGADRIVE,     "Sega MegaDrive"},
            {HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem"},
            {HARDWARE_PREFIX_SEGA_SG1000,        "Sega SG1000"},
            {HARDWARE_PREFIX_SEGA_GAME_GEAR,     "Sega GameGear"},
            {HARDWARE_PREFIX_PCENGINE,           "PC-Engine"},
            {HARDWARE_PREFIX_COLECO,             "Coleco"},
            {HARDWARE_PREFIX_MIDWAY,             "Midway"},
            {HARDWARE_PREFIX_MSX,                "MSX"}
    };

#endif

    BurnPathsInit();
    BurnLibInit();

    // load configuration
    std::string cfgPath = szAppHomePath;
    cfgPath += "/pfba.cfg";
    config = new Config(cfgPath, hardwares);

#ifdef __PSP2__
    // init input
    inp = (Input *) new SDL2Input();
#elif __RPI__
    // init input
    inp = (Input *) new SDL2Input();
#else
    // init input
    inp = (Input *) new SDL2Input();
#endif

    // build roms list
    std::vector<std::string> rom_paths;
    for (int i = 0; i < DIRS_MAX; i++) {
        rom_paths.push_back(config->GetRomPath(i));
    }
    romList = new RomList(utility, rom_paths, hardwares);

    // run gui
    gui = new Gui(renderer, utility, romList, config, inp);
#ifdef __PSP2__ // prevent rom list scrolling lag on psp2
    gui->SetTitleLoadDelay(500);
#endif
    gui->Run();

    BurnLibExit();

    delete (gui);
    delete (utility);
    delete (romList);
    delete (renderer);
    delete (config);
    delete (inp);

#ifdef __PSP2__
    scePowerSetArmClockFrequency(333);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(166);
#endif

    return 0;
}
