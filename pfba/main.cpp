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
int _newlib_heap_size_user = 192 * 1024 * 1024;
#define SCR_W   960
#define SCR_H   544
#elif __3DS__
#define SCR_W   400
#define SCR_H   240
#elif __NX__
#define SCR_W   1280
#define SCR_H   720
#else
#define SCR_W   800
#define SCR_H   600
#endif

Io *io;
Renderer *renderer;
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

    BurnPathsInit();
    BurnLibInit();

    std::vector<Skin::Button> buttons;
#ifdef __PSP2__
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
    sceIoMkdir("ux0:/data/pfba/config", 0777);
    sceIoMkdir("ux0:/data/pfba/config/games", 0777);

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
#endif

#ifdef __SFML__
    std::string shaderPath = szAppHomePath;
    shaderPath += "shaders/sfml";
    renderer = (Renderer *) new SFMLRenderer(SCR_W, SCR_H, shaderPath);
#else
    renderer = (Renderer *) new C2DRenderer(SCR_W, SCR_H);
#endif
    inp = (Input *) new C2DInput(renderer);
    io = (Io *) new C2DIo();

    // load configuration
    std::string cfgPath = szAppHomePath;
    cfgPath += "pfba.cfg";
    config = new Config(cfgPath, renderer);

    // build/init roms list
    romList = new RomList(io, &config->hardwareList, config->GetRomPaths());

    // skin
    int size = config->GetGuiValue(Option::Index::SKIN_FONT_SIZE);
    Skin *skin = new Skin(renderer, szAppSkinPath, size, buttons);

    // run gui
    gui = new Gui(io, renderer, skin, romList, config, inp);
#ifdef __PSP2__ // prevent rom list scrolling lag on psp2
    gui->SetTitleLoadDelay(500);
#endif

    gui->Run();

    BurnLibExit();

    delete (gui);
    delete (romList);
    delete (config);
    delete (inp);
    delete (skin);
    delete (io);
    delete (renderer);

#ifdef __PSP2__
    scePowerSetArmClockFrequency(266);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(111);
#endif

    return 0;
}
