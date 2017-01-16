//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>
#include "sdl_run.h"
#include "video.h"
#include <skeleton/timer.h>

#define BORDER_SIZE 16

extern Video *video;

extern INT32 MakeScreenShot(const char *dest);

static int max_lines = 16;
static int rom_index = 0;
static int option_index = 0;

struct SaveState {
    char path[MAX_PATH];
    char sshot[MAX_PATH];
    Rect rect;
    Texture *texture;
    bool available = false;
};

void Gui::DrawBg() {

    if (skin->tex_bg) {
        renderer->DrawTexture(skin->tex_bg, 0, 0);
    } else {

        Rect window{0, 0, renderer->GetWindowSize().w, renderer->GetWindowSize().h};
        renderer->DrawRect(&window, &GRAY);
        renderer->DrawBorder(&window, &ORANGE);

        if (skin->tex_title) {
            renderer->DrawTexture(skin->tex_title, &rectTitle, true);
        }

        renderer->DrawRect(&rectRomList, &GRAY_LIGHT);
        renderer->DrawBorder(&rectRomList, &ORANGE, false);

        renderer->DrawRect(&rectRomInfo, &GRAY_LIGHT);
        renderer->DrawBorder(&rectRomInfo, &GREEN, false);
    }
}

void Gui::DrawRomInfo(RomList::Rom *rom) {

    if (rom == NULL) {
        return;
    }

    int hw_cfg = config->GetGuiValue(Option::Index::GUI_SHOW_HARDWARE);
    RomList::Hardware hw = romList->hardwares[hw_cfg];
    int show_clones = config->GetGuiValue(Option::Index::GUI_SHOW_CLONES);
    int available = (int) roms.size();
    if (config->GetGuiValue(Option::Index::GUI_SHOW_ALL)) {
        available = show_clones ? hw.available_count : hw.available_count - hw.available_clone_count;
    }
    char str[512];
    sprintf(str, "ROMS: %i / %i", available, (int) roms.size());

    Rect r = rectRomInfo;
    r.x = (r.x + r.w) - skin->font->GetWidth(str) - 16;
    r.y = rectRomInfo.y + rectRomInfo.h - skin->font->GetHeight(str) - 16;
    renderer->DrawFont(skin->font, &r, &WHITE, str, available, roms.size());

    r = rectRomInfo;
    r.x += 16;
    r.y += 16;
    r.w -= 32;

    switch(rom->state) {
        case RomList::RomState::MISSING:
            renderer->DrawFont(skin->font, &r, &WHITE, "STATUS: MISSING");
            break;
        case RomList::RomState::NOT_WORKING:
            renderer->DrawFont(skin->font, &r, &WHITE, "STATUS: NOT WORKING");
            break;
        case RomList::RomState::WORKING:
            renderer->DrawFont(skin->font, &r, &WHITE, "STATUS: WORKING");
            break;
        default:
            break;
    }
    r.y += skin->font->size;

    renderer->DrawFont(skin->font, &r, &WHITE, "SYSTEM: %s", rom->system);
    r.y += skin->font->size;

    renderer->DrawFont(skin->font, &r, &WHITE, "MANUFACTURER: %s", rom->manufacturer);
    r.y += skin->font->size;

    renderer->DrawFont(skin->font, &r, &WHITE, "YEAR: %s", rom->year);
    r.y += skin->font->size;

    renderer->DrawFont(skin->font, &r, &WHITE, "ZIP: %s.ZIP", rom->zip);
    r.y += skin->font->size;

    if (rom->parent) {
        renderer->DrawFont(skin->font, r.x, r.y, "PARENT: %s.ZIP", rom->parent);
        r.y += skin->font->size;
    }

    if (rom->flags & BDF_ORIENTATION_VERTICAL) {
        renderer->DrawFont(skin->font, r.x, r.y, "ORIENTATION: VERTICAL");
        if (rom->flags & BDF_ORIENTATION_FLIPPED) {
            renderer->DrawFont(skin->font, r.x + skin->font->GetWidth("ORIENTATION: VERTICAL"), r.y, " / FLIPPED");
        }
        r.y += skin->font->size;
    }
}

void Gui::DrawRomList() {

    int font_height = skin->font->size + 2;
    max_lines = rectRomList.h / font_height;
    int page = rom_index / max_lines;

    if (romSelected != NULL) {
        delete (romSelected);
        romSelected = NULL;
    }

    Rect rectText = rectRomList;
    rectText.x += 1;
    rectText.w = rectRomList.w - 2;
    rectText.h = font_height + 4;

    for (int i = page * max_lines; i < page * max_lines + max_lines; i++) {

        if (i >= roms.size())
            break;

        RomList::Rom rom = roms[i];

        // set color
        Color color = RED;
        if (rom.state == RomList::RomState::NOT_WORKING) {
            color = ORANGE;
        } else if (rom.state == RomList::RomState::WORKING) {
            color = rom.parent == NULL ? GREEN : YELLOW;
        }
        color.a = 200;

        // set highlight
        if (i == rom_index) {
            Color c{0, 0, 0, 155};
            color.a = 255;
            renderer->DrawRect(&rectText, &c);
            renderer->DrawBorder(&rectText, &color, false);
            romSelected = new RomList::Rom(rom);
            DrawRomInfo(romSelected);
            if (title != NULL) {
                Rect dst = renderer->DrawTexture(title, &rectRomPreview, true);
                renderer->DrawBorder(&dst, &RED, false);
            }
        }

        // draw rom name text
        Rect r = rectText;
        r.x += 6;
        r.w -= 6;
        renderer->DrawFont(skin->font, &r, &color, false, true, rom.name);
        rectText.y += font_height;
    }
}

bool Gui::IsOptionHidden(Option *option) {

    return option->index == Option::Index::ROM_ROTATION
           && romSelected != NULL
           && !(romSelected->flags & BDF_ORIENTATION_VERTICAL);

}

void Gui::DrawOptions(bool isRomCfg, std::vector<Option> *options, int start, int end) {

    Rect rect{32, 32,
              renderer->GetWindowSize().w - 64,
              renderer->GetWindowSize().h - 64
    };
    Rect rect_start = rect;

    // window
    renderer->DrawRect(&rect, &GRAY);
    renderer->DrawBorder(&rect, &GREEN);

    for (int i = start; i < end; i++) {

        if (i >= options->size()) {
            break;
        }

        // draw menu types
        Option *option = &options->at((unsigned long) i);

        if (option->type == Option::Type::MENU) {
            bool new_col = option->index == Option::Index::MENU_JOYPAD;
            if (new_col) {
                rect.x += rect.w / 2;
                rect.y = rect_start.y;
            }
            if (i != start && !new_col) {
                rect.y += 32;
            }
            Rect r = rect;
            r.x += 16;
            r.y += 16;
            r.w = rect.w / 3;
            renderer->DrawFont(skin->font, &r, &ORANGE, option->GetName());
            renderer->color = ORANGE;
            renderer->DrawLine(r.x, r.y + skin->font->size, r.x + r.w, r.y + skin->font->size);
            renderer->color = BLACK;
            rect.y += 32;
        } else {

            // skip rotation option if not needed
            if ((isRomCfg && IsOptionHidden(option))
                    || option->type == Option::Type::HIDDEN) {
                continue;
            }

            // draw selection
            if (i == option_index) {
                Rect sel{rect.x + 190, rect.y + 24, 150, 30};
                renderer->DrawRect(&sel, &GRAY_LIGHT);
                renderer->DrawBorder(&sel, &GREEN);
            }

            renderer->DrawFont(skin->font, rect.x + 16, rect.y + 32, option->GetName());
            Rect sel{rect.x + 196, rect.y + 25, 140, 30};
            if (option->type == Option::Type::INPUT)
                renderer->DrawFont(skin->font, &sel, &WHITE, false, true, "%i", option->value);
            else
                renderer->DrawFont(skin->font, &sel, &WHITE, false, true, option->GetValue());

            rect.y += skin->font->size;
        }
    }
}

int Gui::MessageBox(const char *message, const char *choice1, const char *choice2) {

    int first = 1;
    int index = 0;
    int max_choice = choice2 == NULL ? 1 : 2;

    Rect win{
            renderer->GetWindowSize().w / 4,
            renderer->GetWindowSize().h / 4,
            renderer->GetWindowSize().w / 2,
            renderer->GetWindowSize().h / 2
    };

    Rect buttons[2]{
            {win.x + 16,               win.y + win.h - 64, (win.w / 2) - 32, 32},
            {win.x + (win.w / 2) + 16, win.y + win.h - 64, (win.w / 2) - 32, 32}
    };

    if (choice2 == NULL) {
        buttons[0].x = win.x + (win.w / 2) - (buttons[0].w / 2);
    }

    input->Clear(0);

    while (true) {

        int key = input->Update()[0].state;
        if (key > 0 || first) {

            first = 0;

            if (key & Input::Key::KEY_UP) {
            } else if (key & Input::Key::KEY_DOWN) {
            } else if (key & Input::Key::KEY_LEFT) {
                index--;
                if (index < 0)
                    index = max_choice - 1;
            } else if (key & Input::Key::KEY_RIGHT) {
                index++;
                if (index > max_choice - 1)
                    index = 0;
            } else if (key & Input::Key::KEY_FIRE1) {
                return index;
            } else if (key & Input::Key::KEY_FIRE2) {
                return -1;
            }

            Clear();
            // video should always be initialized here
            video->Render();

            renderer->DrawRect(&win, &GRAY);
            renderer->DrawBorder(&win, &GREEN);

            Rect r_msg = win;
            r_msg.y -= skin->font->GetHeight(message);
            renderer->DrawFont(skin->font, &r_msg, &WHITE, true, true, message);

            renderer->DrawRect(&buttons[0], &GRAY_LIGHT);
            renderer->DrawBorder(&buttons[0], &BLACK, false);
            renderer->DrawFont(skin->font, &buttons[0], &WHITE, true, true, choice1);

            if (max_choice > 1) {
                renderer->DrawRect(&buttons[1], &GRAY_LIGHT);
                renderer->DrawBorder(&buttons[1], &BLACK, false);
                renderer->DrawFont(skin->font, &buttons[1], &WHITE, true, true, choice2);
            }

            renderer->DrawBorder(&buttons[index], &ORANGE);

            Flip();
            renderer->Delay(INPUT_DELAY);
        }
    }

    input->Clear(0);
}

void Gui::RunStatesMenu() {

    int first = 1;
    int save_index = 0;
    const int save_max = 4;

    SaveState saves[4];

    // load states screenshot if any
    for (int i = 0; i < 4; i++) {
        memset(saves[i].path, 0, MAX_PATH);
        memset(saves[i].sshot, 0, MAX_PATH);
        sprintf(saves[i].path, "%s/%s%i.sav", szAppSavePath, romSelected->zip, i);
        sprintf(saves[i].sshot, "%s/%s%i.png", szAppSavePath, romSelected->zip, i);
        saves[i].available = utility->FileExist(saves[i].path);
        saves[i].texture = NULL;
        if (utility->FileExist(saves[i].sshot)) {
            saves[i].texture = renderer->LoadTexture(saves[i].sshot);
        }
    }

    input->Clear(0);

    if (pBurnDraw == NULL) {
        printf("RunOptionMenu: force redraw: RunOneFrame\n");
        // force a frame to be drawn to fba buffer
        bPauseOn = false;
        RunOneFrame(true, 0, 0);
        bPauseOn = true;
        if (pBurnDraw == NULL) {
            printf("RunOptionMenu: pBurnDraw == NULL\n");
        }
    }

    while (true) {

        int key = input->Update()[0].state;
        if (key > 0 || first) {

            first = 0;

            if (key & Input::Key::KEY_UP) {
            } else if (key & Input::Key::KEY_DOWN) {
            } else if (key & Input::Key::KEY_LEFT) {
                save_index--;
                if (save_index < 0)
                    save_index = save_max - 1;
            } else if (key & Input::Key::KEY_RIGHT) {
                save_index++;
                if (save_index >= save_max)
                    save_index = 0;
            } else if (key & Input::Key::KEY_FIRE1) {
                if (saves[save_index].available) {
                    int res = MessageBox("Press FIRE1 to confirm, FIRE2 to cancel",
                                         "Load", "Save");
                    if (res == 0) {
                        printf("StateLoad: %s\n", saves[save_index].path);
                        BurnStateLoad(saves[save_index].path, 1, &DrvInitCallback);
                        break;
                    } else if (res == 1) {
                        printf("StateSave: %s\n", saves[save_index].path);
                        BurnStateSave(saves[save_index].path, 1);
                        res = MakeScreenShot(saves[save_index].sshot);
                        if (res == 0) {
                            if (saves[save_index].texture) {
                                delete (saves[save_index].texture);
                            }
                            saves[save_index].texture = renderer->LoadTexture(saves[save_index].sshot);
                        } else {
                            printf("ERROR: MakeScreenShot = %i\n", res);
                        }
                        break;
                    }
                } else {
                    printf("StateSave: %s\n", saves[save_index].path);
                    BurnStateSave(saves[save_index].path, 1);
                    int res = MakeScreenShot(saves[save_index].sshot);
                    if (res == 0) {
                        if (saves[save_index].texture) {
                            delete (saves[save_index].texture);
                        }
                        saves[save_index].texture = renderer->LoadTexture(saves[save_index].sshot);
                    } else {
                        printf("ERROR: MakeScreenShot = %i\n", res);
                    }
                    break;
                }
            } else if (key & Input::Key::KEY_FIRE2) {
                break;
            }

            Clear();
            video->Render();

            Rect win = renderer->GetWindowSize();
            int r_width = win.w / 4;

            for (int i = 0; i < 4; i++) {
                saves[i].rect.x = (r_width * i) + 16;
                saves[i].rect.y = 16;
                saves[i].rect.w = r_width - 32;
                saves[i].rect.h = r_width - 32;
                renderer->DrawRect(&saves[i].rect, &GRAY);
                if (saves[i].available) {
                    if (saves[i].texture) {
                        renderer->DrawTexture(saves[i].texture, &saves[i].rect);
                    } else {
                        renderer->DrawFont(skin->font, &saves[i].rect, &WHITE, true, true, "NO IMAGE");
                    }
                    Rect r = saves[i].rect;
                    r.y = saves[i].rect.y + saves[i].rect.h - skin->font->size;
                    renderer->DrawFont(skin->font, &r, &ORANGE, true, false, "STATE %i", i);
                } else {
                    renderer->DrawFont(skin->font, &saves[i].rect, &WHITE, true, true, "FIRE1 TO SAVE");
                }
                if (i == save_index) {
                    renderer->DrawBorder(&saves[i].rect, &GREEN);
                    renderer->DrawBorder(&saves[i].rect, &ORANGE, false);
                }
            }

            Flip();
            renderer->Delay(INPUT_DELAY);
        }
    }

    // delete states screenshots if any
    for (int i = 0; i < 4; i++) {
        if (saves[i].texture) {
            delete (saves[i].texture);
        }
    }

    for (int i = 0; i < 3; i++) {
        Clear();
        Flip();
    }

    input->Clear(0);
}

void Gui::RunOptionMenu(bool isRomConfig) {

    int first = 1;
    bool option_changed = false;
    bool stop = false;
    option_index = 1;

    std::vector<Option> *options =
            isRomConfig ? config->GetRomOptions() : config->GetGuiOptions();

    input->Clear(0);
    UpdateInputMapping(false);

    if (GameLooping) {

        if (pBurnDraw == NULL) {
            printf("RunOptionMenu: force redraw: RunOneFrame\n");
            // force a frame to be drawn to fba buffer
            bPauseOn = false;
            RunOneFrame(true, 0, 0);
            bPauseOn = true;
            if (pBurnDraw == NULL) {
                printf("RunOptionMenu: pBurnDraw == NULL\n");
            }
        }

        options->insert(options->begin(), Option("EXIT", {"GO"}, 0, OPTION_EXIT));
        options->insert(options->begin(), Option("STATES", {"GO"}, 0, OPTION_STATES));
        options->insert(options->begin(), Option("RETURN", {"GO"}, 0, OPTION_RETURN));
        options->insert(options->begin(),
                        Option(romSelected->name, {romSelected->name}, 0, (Option::Index) 0, Option::Type::MENU));
    } else {
        // change "rom" title menu to game name
        int index = config->GetOptionPos(options, Option::Index::MENU_ROM_OPTIONS);
        options->at(index).SetName(isRomConfig ? romSelected->name : "Default roms options");
    }

    while (true) {

        int key = input->Update()[0].state;
        if (key > 0 || first) {

            first = 0;

            if (key & Input::Key::KEY_UP) {
                option_index--;
                if (option_index < 0)
                    option_index = config->GetOptionPos(options, Option::Index::MENU_KEYBOARD - 1);
                // skip menus and submenus
                Option *option = &options->at((unsigned long) option_index);
                while (option->type == Option::Type::MENU
                       || option->type == Option::Type::HIDDEN
                       || (isRomConfig && IsOptionHidden(option))) {
                    option_index--;
                    if (option_index < 0)
                        option_index = config->GetOptionPos(options, Option::Index::MENU_KEYBOARD - 1);
                    option = &options->at((unsigned long) option_index);
                }
            } else if (key & Input::Key::KEY_DOWN) {
                option_index++;
                if (option_index >= config->GetOptionPos(options, Option::Index::MENU_KEYBOARD))
                    option_index = 0;
                // skip menus and submenus
                Option *option = &options->at((unsigned long) option_index);
                while (option->type == Option::Type::MENU
                       || option->type == Option::Type::HIDDEN
                       || (isRomConfig && IsOptionHidden(option))) {
                    option_index++;
                    if (option_index >= config->GetOptionPos(options, Option::Index::MENU_KEYBOARD))
                        option_index = 0;
                    option = &options->at((unsigned long) option_index);
                }
            } else if (key & Input::Key::KEY_LEFT) {
                option_changed = true;
                Option *option = &options->at((unsigned long) option_index);
                if (option->type == Option::Type::INTEGER) {
                    option->Prev();
                    switch (option->index) {

                        case Option::Index::GUI_SHOW_CLONES:
                        case Option::Index::GUI_SHOW_ALL:
                        case Option::Index::GUI_SHOW_HARDWARE:
                            FilterRoms();
                            break;

                        case Option::ROM_ROTATION:
                        case Option::Index::ROM_SCALING:
                            if (GameLooping && video) {
                                video->Scale();
                            }
                            break;

                        case Option::Index::ROM_FILTER:
                            if (GameLooping && video) {
                                video->Filter(option->value);
                            }
                            break;

                        case Option::Index::ROM_SHADER:
                            if (GameLooping) {
                                renderer->SetShader(option->value);
                            }
                            break;

                        default:
                            break;
                    }
                }
            } else if (key & Input::Key::KEY_RIGHT) {
                option_changed = true;
                Option *option = &options->at((unsigned long) option_index);
                if (option->type == Option::Type::INTEGER) {
                    option->Next();
                    switch (option->index) {

                        case Option::Index::GUI_SHOW_CLONES:
                        case Option::Index::GUI_SHOW_ALL:
                        case Option::Index::GUI_SHOW_HARDWARE:
                            FilterRoms();
                            option_changed = true;
                            break;

                        case Option::ROM_ROTATION:
                        case Option::Index::ROM_SCALING:
                            if (GameLooping && video) {
                                video->Scale();
                            }
                            break;

                        case Option::Index::ROM_FILTER:
                            if (GameLooping && video) {
                                video->Filter(option->value);
                            }
                            break;

                        case Option::Index::ROM_SHADER:
                            if (GameLooping) {
                                renderer->SetShader(option->value);
                            }
                            break;

                        default:
                            break;
                    }
                }
            } else if (key & Input::Key::KEY_FIRE1) {
                Option *option = &options->at((unsigned long) option_index);
                if (option->type == Option::Type::INPUT) {
                    int btn = GetButton();
                    if (btn >= 0) {
                        option->value = btn;
                        option_changed = true;
                    }
                } else if (option->index == OPTION_EXIT) {
                    stop = true;
                    break;
                } else if (option->index == OPTION_RETURN) {
                    break;
                } else if (option->index == OPTION_STATES) {
                    RunStatesMenu();
                }
            } else if (key & Input::Key::KEY_FIRE2
                       || (key & Input::Key::KEY_START && !isRomConfig)
                       || (key & Input::Key::KEY_COIN && isRomConfig)) {
                break;
            }

            Clear();
            if (GameLooping) {
                video->Render();
            } else {
                DrawBg();
                DrawRomList();
            }

            int end = config->GetOptionPos(options, Option::Index::MENU_KEYBOARD);
            DrawOptions(isRomConfig, options, 0, end);

            Flip();
            renderer->Delay(INPUT_DELAY);
        }
    }

    if (GameLooping) {
        options->erase(options->begin(), options->begin() + 4);
        for (int i = 0; i < 3; i++) {
            Clear();
            Flip();
        }
    } else {
        // restore "rom" title menu for config save
        int index = config->GetOptionPos(options, Option::Index::MENU_ROM_OPTIONS);
        options->at(index).SetName("ROM");
    }

    if (option_changed) {
        if (isRomConfig) {
            config->Save(romSelected);
        } else {
            config->Save();
        }
    }

    if (stop) {
        GameLooping = false;
    }

    UpdateInputMapping(isRomConfig);
    input->Clear(0);
}

void Gui::Run() {

    Clear();
    DrawBg();
    DrawRomList();
    Flip();

    Timer *timer_input = new Timer();
    Timer *timer_load = new Timer();

    UpdateInputMapping(false);

    while (!quit) {

        int key = input->Update()[0].state;
        if (key > 0) {
            if (key & Input::Key::KEY_UP) {
                rom_index--;
                if (rom_index < 0)
                    rom_index = (int) (roms.size() - 1);
                TitleFree();
            } else if (key & Input::Key::KEY_DOWN) {
                rom_index++;
                if (rom_index >= roms.size())
                    rom_index = 0;
                TitleFree();
            } else if (key & Input::Key::KEY_RIGHT) {
                rom_index += max_lines;
                if (rom_index >= roms.size())
                    rom_index = (int) (roms.size() - 1);
                TitleFree();
            } else if (key & Input::Key::KEY_LEFT) {
                rom_index -= max_lines;
                if (rom_index < 0)
                    rom_index = 0;
                TitleFree();
            } else if (key & Input::Key::KEY_FIRE1) {
                if (romSelected != NULL
                    && romSelected->state != RomList::RomState::MISSING) {
                    RunRom(romSelected);
                }
            } else if (key & Input::Key::KEY_START) {
                RunOptionMenu();
                if (title != NULL) {
                    // refresh preview/title image
                    DrawBg();
                    DrawRomList();
                    renderer->DrawTexture(title, &rectRomPreview, true);
                    Flip();
                }
            } else if (key & Input::Key::KEY_COIN) {
                if (romSelected != NULL) {
                    config->Load(romSelected);
                    RunOptionMenu(true);
                    if (title != NULL) {
                        // refresh preview/title image
                        DrawBg();
                        DrawRomList();
                        renderer->DrawTexture(title, &rectRomPreview, true);
                        Flip();
                    }
                }
            } else if (key & Input::Key::KEY_QUIT) {
                quit = true;
            }

            Clear();
            DrawBg();
            DrawRomList();
            Flip();

            if (timer_input->GetSeconds() > 6) {
                renderer->Delay(INPUT_DELAY / 5);
            } else if (timer_input->GetSeconds() > 2) {
                renderer->Delay(INPUT_DELAY / 2);
            } else {
                renderer->Delay(INPUT_DELAY);
            }
            timer_load->Reset();
        } else {
            if (romSelected != NULL && !title_loaded
                && timer_load->GetMillis() >= title_delay) {
                if (TitleLoad(romSelected)) {
                    DrawBg();
                    DrawRomList();
                    Flip();
                }
                title_loaded = 1;
                timer_load->Reset();
            }
            timer_input->Reset();
        }
    }

    delete (timer_input);
    delete (timer_load);
}

Gui::Gui(Renderer *rdr, Utility *util, RomList *rList, Config *cfg, Input *in) {

    renderer = rdr;
    utility = util;
    romList = rList;
    config = cfg;
    input = in;

    // create skin
    skin = new Skin(szAppSkinPath, renderer);
    skin->font->color = WHITE;

    // set drawing rects
    Rect size = renderer->GetWindowSize();

    // title rect
    rectTitle.x = BORDER_SIZE;
    rectTitle.y = BORDER_SIZE;
    rectTitle.w = size.w / 2;
    rectTitle.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rectTitle.h = size.h / 4;
    rectTitle.h -= BORDER_SIZE;
    // rom list rect
    rectRomList.x = BORDER_SIZE;
    rectRomList.y = size.h / 4;
    rectRomList.y += BORDER_SIZE;
    rectRomList.w = size.w / 2;
    rectRomList.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rectRomList.h = (size.h / 4) * 3;
    rectRomList.h -= BORDER_SIZE * 2;
    // rom preview image rect
    rectRomPreview.x = (size.w / 2);
    rectRomPreview.x += BORDER_SIZE / 2;
    rectRomPreview.y = BORDER_SIZE;
    rectRomPreview.w = (size.w / 2);
    rectRomPreview.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rectRomPreview.h = (size.h / 2);
    rectRomPreview.h -= BORDER_SIZE + BORDER_SIZE / 2;
    // rom information rect
    rectRomInfo.x = (size.w / 2);
    rectRomInfo.x += BORDER_SIZE / 2;
    rectRomInfo.y = (size.h / 2) + BORDER_SIZE / 2;
    rectRomInfo.w = (size.w / 2);
    rectRomInfo.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rectRomInfo.h = (size.h / 2);
    rectRomInfo.h -= BORDER_SIZE + BORDER_SIZE / 2;

    // filter roms
    FilterRoms();
}

void Gui::TitleFree() {
    if (title) {
        delete (title);
        title = NULL;
    }
    title_loaded = 0;
}

int Gui::TitleLoad(RomList::Rom *rom) {

    TitleFree();

    char path[MAX_PATH];
    sprintf(path, "%s/%s.png", szAppPreviewPath, rom->zip);
    if (utility->FileExist(path)) {
        title = renderer->LoadTexture(path);
        return title != NULL;
    } else if (rom->parent) {
        memset(path, 0, MAX_PATH);
        sprintf(path, "%s/%s.png", szAppPreviewPath, rom->parent);
        if (utility->FileExist(path)) {
            title = renderer->LoadTexture(path);
            return title != NULL;
        }
    }
    return 0;
}

void Gui::FilterRoms() {

    roms.clear();

    int showClone = config->GetGuiValue(Option::Index::GUI_SHOW_CLONES);
    int showAll = config->GetGuiValue(Option::Index::GUI_SHOW_ALL);
    int showHardwareCfg = config->GetGuiValue(Option::Index::GUI_SHOW_HARDWARE);
    int showHardware = romList->hardwares[showHardwareCfg].prefix;

    remove_copy_if(romList->list.begin(), romList->list.end(), back_inserter(roms),
                   [showAll, showClone, showHardware](const RomList::Rom r) {
                       return !showAll && r.state != RomList::RomState::WORKING
                              || !showClone && r.parent != NULL
                              || showHardware != HARDWARE_PREFIX_ALL && !RomList::IsHardware(r.hardware, showHardware);
                   });

    rom_index = 0;
    TitleFree();
}

void Gui::RunRom(RomList::Rom *rom) {

    if (rom == NULL) {
        return;
    }

    char path[MAX_PATH];
    for (int i = 0; i < DIRS_MAX; i++) {
        if (strlen(config->GetRomPath(i)) > 0) {
            sprintf(path, "%s%s.zip", config->GetRomPath(i), rom->zip);
            printf("%s\n", path);
            if (utility->FileExist(path))
                break;
        }
    }

    if (!utility->FileExist(path)) {
        printf("RunRom: rom not found: `%s`\n", rom->zip);
        return;
    }

    printf("RunRom: %s\n", path);
    for (nBurnDrvSelect[0] = 0; nBurnDrvSelect[0] < nBurnDrvCount; nBurnDrvSelect[0]++) {
        nBurnDrvActive = nBurnDrvSelect[0];
        if (strcasecmp(rom->zip, BurnDrvGetTextA(DRV_NAME)) == 0)
            break;
    }

    if (nBurnDrvActive >= nBurnDrvCount) {
        printf("RunRom: driver not found\n");
        return;
    }

    // load rom settings
    printf("RunRom: config->LoadRom(%s)\n", rom->zip);
    config->Load(rom);

    printf("RunRom: RunEmulator: start\n");
    RunEmulator(this, nBurnDrvActive);

    printf("RunRom: RunEmulator: return\n");
    mode = List;
}

Input *Gui::GetInput() {
    return input;
}

Renderer *Gui::GetRenderer() {
    return renderer;
}

Skin *Gui::GetSkin() {
    return skin;
}

Config *Gui::GetConfig() {
    return config;
}

void Gui::SetTitleLoadDelay(int delay) {
    title_delay = delay;
}

int Gui::GetButton() {

    Rect box{
            renderer->GetWindowSize().w / 4,
            renderer->GetWindowSize().h / 4,
            renderer->GetWindowSize().w / 2,
            renderer->GetWindowSize().h / 2
    };

    Timer *timer = new Timer();

    input->Clear(0);

    while (true) {

        int btn = input->GetButton(0);
        if (btn >= 0 || timer->GetSeconds() >= 3) {
            return btn;
        }

        renderer->Clear();
        renderer->DrawRect(&box, &GRAY);
        renderer->DrawBorder(&box, &GREEN);
        renderer->DrawFont(skin->font, &box, &WHITE, true, true, "PRESS A BUTTON (%i)", 3 - timer->GetSeconds());
        Flip();
    }
}

void Gui::UpdateInputMapping(bool isRomConfig) {

    if (isRomConfig) {
        input->SetKeyboardMapping(config->GetRomPlayerInputKeys(0));
        int deadzone = 2000 + config->GetRomValue(Option::Index::JOY_DEADZONE) * 2000;
        input->SetJoystickMapping(0, config->GetRomPlayerInputButtons(0), deadzone);
        input->players[0].axis_lx = config->GetRomValue(Option::Index::JOY_AXIS_LX);
        input->players[0].axis_ly = config->GetRomValue(Option::Index::JOY_AXIS_LY);
        input->players[0].axis_rx = config->GetRomValue(Option::Index::JOY_AXIS_RX);
        input->players[0].axis_ry = config->GetRomValue(Option::Index::JOY_AXIS_RY);
    } else {
        input->SetKeyboardMapping(config->GetGuiPlayerInputKeys(0));
        int deadzone = 2000 + config->GetGuiValue(Option::Index::JOY_DEADZONE) * 2000;
        input->SetJoystickMapping(0, config->GetGuiPlayerInputButtons(0), deadzone);
        input->players[0].axis_lx = config->GetGuiValue(Option::Index::JOY_AXIS_LX);
        input->players[0].axis_ly = config->GetGuiValue(Option::Index::JOY_AXIS_LY);
        input->players[0].axis_rx = config->GetGuiValue(Option::Index::JOY_AXIS_RX);
        input->players[0].axis_ry = config->GetGuiValue(Option::Index::JOY_AXIS_RY);
    }
}

void Gui::Clear() {
    renderer->Clear();
}

void Gui::Flip() {
    renderer->Flip();
}

Gui::~Gui() {
    delete (skin);

}
