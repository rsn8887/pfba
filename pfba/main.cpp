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

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#include <psp2/psp2_utility.h>
#include <sdl2/sdl2_input.h>
int _newlib_heap_size_user = 192 * 1024 * 1024;
#elif __SDL2__
#include <sdl2/sdl2_input.h>
#elif __SFML__
#include <sfml/sfml_input.h>
#endif

Renderer *renderer;
Utility *utility;
Config *config;
RomList *romList;
Gui *gui;
Input *inp;
Skin *skin;

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

    std::vector<Skin::Button> buttons;
#ifdef __PSP2__
#ifdef __PSP2_DEBUG__
    psp2shell_init(3333, 0);
#endif
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);

    // create needed directories
    sceIoMkdir("ux0:/data/pfba", 0777);
    sceIoMkdir("ux0:/data/pfba/saves", 0777);
    sceIoMkdir("ux0:/data/pfba/configs", 0777);
    sceIoMkdir("ux0:/data/pfba/hiscore", 0777);
    sceIoMkdir("ux0:/data/pfba/samples", 0777);
    sceIoMkdir("ux0:/data/pfba/previews", 0777);
    sceIoMkdir("ux0:/data/pfba/blend", 0777);
    sceIoMkdir("ux0:/data/pfba/roms", 0777);

    buttons.push_back({8, "UP"});
    buttons.push_back({6, "DOWN"});
    buttons.push_back({7, "LEFT"});
    buttons.push_back({9, "RIGHT"});
    buttons.push_back({0, "TRIANGLE"});
    buttons.push_back({1, "CIRCLE"});
    buttons.push_back({2, "CROSS"});
    buttons.push_back({3, "SQUARE"});
    buttons.push_back({4, "L"});
    buttons.push_back({5, "R"});
    buttons.push_back({10, "SELECT"});
    buttons.push_back({11, "START"});

    renderer = (Renderer *) new PSP2Renderer(960, 544);
    utility = (Utility*)new PSP2Utility();
#elif __SDL2__
    renderer = (Renderer *) new SDL2Renderer(960, 544);
    utility = new Utility();
#elif __SFML__
    renderer = (Renderer *) new SFMLRenderer(960, 544);
    utility = new Utility();
#endif

    BurnPathsInit();
    BurnLibInit();

    // load configuration
    std::string cfgPath = szAppHomePath;
    cfgPath += "/pfba.cfg";
    config = new Config(cfgPath);

    // init input
#ifdef __PSP2__
    inp = (Input *) new SDL2Input();
#elif __SDL2__
    inp = (Input *) new SDL2Input();
#elif __SFML__
    inp = (Input *) new SFMLInput((SFMLRenderer*)renderer);
#endif

    // build/init roms list
    romList = new RomList(utility, &config->hardwareList, config->GetRomPaths());

    // skin
    Skin *skin = new Skin(renderer, szAppSkinPath, buttons);

    // run gui
    gui = new Gui(renderer, skin, utility, romList, config, inp);
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
    scePowerSetArmClockFrequency(266);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(111);
#endif

    return 0;
}
