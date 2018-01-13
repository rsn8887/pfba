//
// Created by cpasjuste on 22/11/16.
//

#ifndef _GUI_H_
#define _GUI_H_

#include <C2D.h>

#include "skin.h"
#include "romlist.h"
#include "config.h"
#include "menu.h"

class Config;
class Option;

#define INPUT_DELAY 150

#define STANDARD_LAYOUT

class Gui {

public:

    Gui(Io* io, Renderer *rdr, Skin *skin, RomList *rList, Config *cfg, Input *input);

    ~Gui();


    void Run();
    void RunRom(RomList::Rom *rom);
    int MessageBox(const char *message, const char *choice1, const char *choice2);
    void DrawBg();
    void DrawRomList();
    void DrawOptions(bool isRomCfg, Menu *_menu);
    void DrawRomInfo(RomList::Rom *rom);
    void RunOptionMenu(bool isRomConfig = false);
    void RunStatesMenu();
    int GetButton();
    int TitleLoad(RomList::Rom *rom);
    void TitleFree();
    void SetTitleLoadDelay(int delay);

    const Rect GetRectTitle();
    const Rect GetRectRomList();

#ifdef STANDARD_LAYOUT
    const Rect GetRectRomPreview();
#else
    const Rect GetRectRomTitle();
#endif
    const Rect GetRectRomInfo();

    void UpdateInputMapping(bool isRomCfg);

    void Clear();
    void Flip();

    Renderer *GetRenderer();
    Skin *GetSkin();
    Config *GetConfig();
    Input *GetInput();

private:

    bool IsOptionHidden(Option *option);

    Io *io = NULL;
    Config *config = NULL;
    Renderer *renderer = NULL;
    Skin *skin = NULL;
    Input *input = NULL;
    Menu *menu_gui = NULL;
    Menu *menu_rom = NULL;
    Menu *menu_current = NULL;

#ifndef STANDARD_LAYOUT
    Texture *preview = NULL;
#endif
    Texture *title = NULL;
    int title_loaded = 0;
    int title_delay = 0;

    RomList *romList = NULL;
    std::vector<RomList::Rom> roms;
    RomList::Rom *romSelected = NULL;
    void FilterRoms();

    bool quit = false;
};

#endif //_GUI_H_
