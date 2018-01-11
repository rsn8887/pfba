//
// Created by cpasjuste on 22/11/16.
//
#include <skeleton/timer.h>
#include <algorithm>
#include "run.h"
#include "video.h"
#include "menu.h"

#define BORDER_SIZE 16

extern Video *video;

extern INT32 MakeScreenShot(const char *dest);

#ifdef STANDARD_LAYOUT
static int max_lines = 16;
#else
static int max_lines = 22;
#endif
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

    if (skin->tex_bg->available) {
        skin->tex_bg->Draw(0, 0);
    } else {

        Rect window{0, 0, renderer->width, renderer->height};
        renderer->DrawRect(window, C2D_COL_GRAY);
        renderer->DrawRect(window, C2D_COL_ORANGE, false);

#ifdef STANDARD_LAYOUT
        if (skin->tex_title->available) {
            skin->tex_title->Draw(GetRectTitle(), true);
        }
#endif
        renderer->DrawRect(GetRectRomList(), C2D_COL_GRAY_LIGHT);
        renderer->DrawRect(GetRectRomList(), C2D_COL_ORANGE, false);

#ifdef STANDARD_LAYOUT
        renderer->DrawRect(GetRectRomInfo(), C2D_COL_GRAY_LIGHT);
        renderer->DrawRect(GetRectRomInfo(), C2D_COL_GREEN, false);
#endif
    }
}

void Gui::DrawRomInfo(RomList::Rom *rom) {

    if (rom == NULL) {
        return;
    }

    int hw_cfg = config->GetGuiValue(Option::Index::GUI_SHOW_HARDWARE);
    RomList::Hardware hw = config->hardwareList[hw_cfg];
    int show_clones = config->GetGuiValue(Option::Index::GUI_SHOW_CLONES);
    int available = (int) roms.size();
    if (config->GetGuiValue(Option::Index::GUI_SHOW_ALL)) {
        available = show_clones ? hw.available_count : hw.available_count - hw.available_clone_count;
    }
    char str[512];
    sprintf(str, "ROMS: %i / %i", available, (int) roms.size());

    Rect r = GetRectRomInfo();
    r.x = (r.x + r.w) - skin->font->GetWidth(str) - 16;
    r.y = r.y + r.h - skin->font->GetHeight(str) - 16;
    skin->font->Draw(r, C2D_COL_WHITE, str, available, roms.size());

    r = GetRectRomInfo();
    r.x += 16;
    r.y += 16;
    r.w -= 32;

    switch (rom->state) {
        case RomList::RomState::MISSING:
            skin->font->Draw(r, C2D_COL_WHITE, "STATUS: MISSING");
            break;
        case RomList::RomState::NOT_WORKING:
            skin->font->Draw(r, C2D_COL_WHITE, "STATUS: NOT WORKING");
            break;
        case RomList::RomState::WORKING:
            skin->font->Draw(r, C2D_COL_WHITE, "STATUS: WORKING");
            break;
        default:
            break;
    }
    r.y += skin->font->size;

    skin->font->Draw(r, C2D_COL_WHITE, "SYSTEM: %s", rom->system);
    r.y += skin->font->size;

    skin->font->Draw(r, C2D_COL_WHITE, "MANUFACTURER: %s", rom->manufacturer);
    r.y += skin->font->size;

    skin->font->Draw(r, C2D_COL_WHITE, "YEAR: %s", rom->year);
    r.y += skin->font->size;

    skin->font->Draw(r, C2D_COL_WHITE, "ZIP: %s.ZIP", rom->zip);
    r.y += skin->font->size;

    if (rom->parent) {
        skin->font->Draw(r.x, r.y, "PARENT: %s.ZIP", rom->parent);
        r.y += skin->font->size;
    }

    if (rom->flags & BDF_ORIENTATION_VERTICAL) {
        skin->font->Draw(r.x, r.y, "ORIENTATION: VERTICAL");
        if (rom->flags & BDF_ORIENTATION_FLIPPED) {
            skin->font->Draw(r.x + skin->font->GetWidth("ORIENTATION: VERTICAL"), r.y, " / FLIPPED");
        }
    }
}

void Gui::DrawRomList() {

    int font_height = skin->font->size + 4;
    max_lines = GetRectRomList().h / font_height;
    int page = rom_index / max_lines;

    if (romSelected != NULL) {
        delete (romSelected);
        romSelected = NULL;
    }

    Rect rectText = GetRectRomList();
    rectText.x += 1;
    rectText.w -= 2;
    rectText.h = font_height;

    for (int i = page * max_lines; i < page * max_lines + max_lines; i++) {

        if (i >= roms.size())
            break;

        RomList::Rom rom = roms[i];

        // set color
        Color color = C2D_COL_RED;
        if (rom.state == RomList::RomState::NOT_WORKING) {
            color = C2D_COL_ORANGE;
        } else if (rom.state == RomList::RomState::WORKING) {
            color = rom.parent == NULL ? C2D_COL_GREEN : C2D_COL_YELLOW;
        }
        color.a = 200;

        // set highlight
        if (i == rom_index) {
            Color c{0, 0, 0, 155};
            color.a = 255;
            renderer->DrawRect(rectText, c);
            renderer->DrawRect(rectText, color, false);
            romSelected = new RomList::Rom(rom);
#ifdef STANDARD_LAYOUT
            DrawRomInfo(romSelected);
            if (title != NULL) {
                Rect dst = title->Draw(GetRectRomPreview(), true);
                renderer->DrawRect(dst, C2D_COL_RED, false);
            }
#else
            if (title != NULL) {
                Rect dst = title->Draw(GetRectRomTitle(), true);
                renderer->DrawRect(dst, C2D_COL_ORANGE, false);
            }
            else {
                Rect dst = GetRectRomTitle();
                renderer->DrawRect(dst, C2D_COL_GRAY_LIGHT);
                renderer->DrawRect(dst, C2D_COL_ORANGE, false);
            }
            if (preview != NULL) {
                Rect dst = preview->Draw(GetRectRomInfo(), true);
                renderer->DrawRect(dst, C2D_COL_ORANGE, false);
            }
            else {
                Rect dst = GetRectRomInfo();
                renderer->DrawRect(dst, C2D_COL_GRAY_LIGHT);
                renderer->DrawRect(dst, C2D_COL_ORANGE, false);
            }
#endif
        }

        // draw rom name text
        Rect r = rectText;
        r.x += 6;
        r.w -= 6;
        skin->font->Draw(r, color, false, true, rom.name);
        rectText.y += font_height;
    }
}

bool Gui::IsOptionHidden(Option *option) {

    return option->index == Option::Index::ROM_ROTATION
           && romSelected != NULL
           && !(romSelected->flags & BDF_ORIENTATION_VERTICAL);
}

void Gui::DrawOptions(bool isRomCfg, Menu *m) {

    Rect rect{32, 32,
              renderer->width - 64,
              renderer->height - 64
    };

    // window
    renderer->DrawRect(rect, C2D_COL_GRAY);
    renderer->DrawRect(rect, C2D_COL_GREEN, false);

    // title
    Rect r = rect;
    r.x += 16;
    r.y += 16;
    r.w = rect.w / 2;
    skin->font->Draw(r, C2D_COL_ORANGE, m->title.c_str());
    renderer->color = C2D_COL_ORANGE;
    renderer->DrawLine(r.x, r.y + skin->font->size, r.x + r.w, r.y + skin->font->size);
    renderer->color = C2D_COL_BLACK;
    rect.y += 16;

    std::vector<Option> *options =
            isRomCfg ? config->GetRomOptions() : config->GetGuiOptions();

    // draw options
    for (int i = 0; i < m->option_ids.size(); i++) {

        // draw menu types
        Option *option = config->GetOption(options, m->option_ids[i]);
        if (option == NULL) {
            continue;
        }

        // skip rotation option if not needed
        if ((isRomCfg && IsOptionHidden(option))
            || option->flags & Option::Type::HIDDEN) {
            continue;
        }

        // option "title"
        skin->font->Draw(rect.x + 16, rect.y + 32, option->GetName());

        // draw selection
        if (i == option_index) {
            int width = rect.w / 6;
            Rect sel{rect.x + rect.w / 3, rect.y + 25, width, skin->font->size + 6};
            renderer->DrawRect(sel, C2D_COL_GRAY_LIGHT);
            renderer->DrawRect(sel, C2D_COL_GREEN, false);
        }

        // option value
        Rect pos{rect.x + (rect.w / 3) + 8, rect.y + 25, rect.w / 3, skin->font->size + 6};

        if (option->flags == Option::Type::INPUT) {
            Skin::Button *button = skin->GetButton(option->value);
            if (button) {
                if (button->texture) {
                    Rect r = pos;
                    r.w = r.h = skin->font->size;
                    r.y += 3; // TODO: fix DrawTexture ?
                    button->texture->Draw(r);
                } else {
                    skin->font->Draw(pos, C2D_COL_WHITE, false, true, "%s", button->name.c_str());
                }
            } else {
                skin->font->Draw(pos, C2D_COL_WHITE, false, true, "%i", option->value);
            }
            rect.y += skin->font->size + 4;
        } else {
            skin->font->Draw(pos, C2D_COL_WHITE, false, true, option->GetValue());
            rect.y += skin->font->size + 2;
        }
    }

    // draw "submenus"
    rect.y += 16;
    for (int i = 0; i < m->childs.size(); i++) {
        if (m->childs[i]->title == "RETURN") {
            rect.y += 16;
        }
        // draw selection
        if (i + m->option_ids.size() == option_index) {
            int width = rect.w / 6;
            Rect sel{rect.x + rect.w / 3, rect.y + 25, width, skin->font->size + 6};
            renderer->DrawRect(sel, C2D_COL_GRAY_LIGHT);
            renderer->DrawRect(sel, C2D_COL_GREEN, false);
        }

        skin->font->Draw(rect.x + 16, rect.y + 32, m->childs[i]->title.c_str());
        skin->font->Draw(rect.x + 16, rect.y + 32, m->childs[i]->title.c_str());
        Rect pos{rect.x + (rect.w / 3) + 8, rect.y + 25, rect.w / 3, skin->font->size + 6};
        skin->font->Draw(pos, C2D_COL_WHITE, false, true, "GO");
        rect.y += skin->font->size + 2;
    }
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
        saves[i].available = io->Exist(saves[i].path);
        saves[i].texture = NULL;
        if (io->Exist(saves[i].sshot)) {
            saves[i].texture = (Texture *) new C2DTexture(renderer, saves[i].sshot);
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

        int key = input->Update(0)[0].state;
        if (key > 0 || first) {

            first = 0;

            if (key & Input::Key::KEY_LEFT) {
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
                            saves[save_index].texture = (Texture *) new C2DTexture(renderer, saves[save_index].sshot);
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
                        saves[save_index].texture = (Texture *) new C2DTexture(renderer, saves[save_index].sshot);
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

            int r_width = renderer->width / 4;

            for (int i = 0; i < 4; i++) {
                saves[i].rect.x = (r_width * i) + 16;
                saves[i].rect.y = 16;
                saves[i].rect.w = r_width - 32;
                saves[i].rect.h = r_width - 32;
                renderer->DrawRect(saves[i].rect, C2D_COL_GRAY);
                if (saves[i].available) {
                    if (saves[i].texture) {
                        saves[i].texture->Draw(saves[i].rect);
                    } else {
                        skin->font->Draw(saves[i].rect, C2D_COL_WHITE, true, true, "NO IMAGE");
                    }
                    Rect r = saves[i].rect;
                    r.y = saves[i].rect.y + saves[i].rect.h - skin->font->size;
                    skin->font->Draw(r, C2D_COL_ORANGE, true, false, "STATE %i", i);
                } else {
                    skin->font->Draw(saves[i].rect, C2D_COL_WHITE, true, true, "FIRE1 TO SAVE");
                }
                if (i == save_index) {
                    renderer->DrawRect(saves[i].rect, C2D_COL_GREEN, false);
                    saves[i].rect.Scale(1);
                    renderer->DrawRect(saves[i].rect, C2D_COL_ORANGE, false);
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
    option_index = 0;

    std::vector<Option> *options = isRomConfig ? config->GetRomOptions() : config->GetGuiOptions();
    menu_current = isRomConfig ? menu_rom : menu_gui;
    menu_current->title = isRomConfig ? romSelected->name : "Options";

    input->Clear(0);

    if (GameLooping) {
        // if frameskip is enabled, we may get a black buffer,
        // force a frame to be drawn
        if (pBurnDraw == NULL) {
            bPauseOn = false;
            RunOneFrame(true, 0, 0);
            bPauseOn = true;
        }
        // add custom in game menus
        menu_current->AddChild("RETURN");
        menu_current->AddChild("STATES");
        menu_current->AddChild("EXIT");
    }

    int item_count = (int) (menu_current->childs.size() + menu_current->option_ids.size());

    while (true) {

        int key = input->Update(0)[0].state;
        if (key > 0 || first) {

            first = 0;

            if (key & Input::Key::KEY_UP) {
                option_index--;
                if (option_index < 0)
                    option_index = item_count - 1;
                if (option_index < menu_current->option_ids.size() && isRomConfig
                    && IsOptionHidden(config->GetOption(options, menu_current->option_ids[option_index]))) {
                    option_index--;
                    if (option_index < 0)
                        option_index = item_count - 1;
                }
            } else if (key & Input::Key::KEY_DOWN) {
                option_index++;
                if (option_index >= item_count)
                    option_index = 0;
                if (option_index < menu_current->option_ids.size() && isRomConfig
                    && IsOptionHidden(config->GetOption(options, menu_current->option_ids[option_index]))) {
                    option_index++;
                    if (option_index >= item_count)
                        option_index = 0;
                }
            } else if (key & Input::Key::KEY_LEFT && option_index < menu_current->option_ids.size()) {
                option_changed = true;
                Option *option = config->GetOption(options, menu_current->option_ids[option_index]);
                if (option->flags == Option::Type::INTEGER) {
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
            } else if (key & Input::Key::KEY_RIGHT && option_index < menu_current->option_ids.size()) {
                option_changed = true;
                Option *option = config->GetOption(options, menu_current->option_ids[option_index]);
                if (option->flags == Option::Type::INTEGER) {
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
                if (option_index < menu_current->option_ids.size()) {
                    Option *option = config->GetOption(options, menu_current->option_ids[option_index]);
                    if (option->flags == Option::Type::INPUT) {
                        int btn = GetButton();
                        if (btn >= 0) {
                            option->value = btn;
                            option_changed = true;
                        }
                    }
                } else {
                    Menu *menu = menu_current->childs[option_index - menu_current->option_ids.size()];
                    if (menu->title == "EXIT") {
                        stop = true;
                        break;
                    } else if (menu->title == "STATES") {
                        RunStatesMenu();
                    } else if (menu->title == "RETURN") {
                        break;
                    } else {
                        menu_current = menu;
                        item_count = (int) (menu_current->childs.size() + menu_current->option_ids.size());
                        option_index = 0;
                    }
                }
            } else if (key & Input::Key::KEY_FIRE2
                       || (key & Input::Key::KEY_MENU1 && !isRomConfig)
                       || (key & Input::Key::KEY_MENU2 && isRomConfig)) {
                if (menu_current->parent == NULL) {
                    break;
                } else {
                    menu_current = menu_current->parent;
                    item_count = (int) (menu_current->childs.size() + menu_current->option_ids.size());
                    option_index = 0;
                }
            }

            Clear();
            if (GameLooping) {
                video->Render();
            } else {
                DrawBg();
                DrawRomList();
            }

            // disable shader for ui transparency
            int shader = renderer->shaders->current;
            if (GameLooping) {
                renderer->SetShader(0);
            }

            DrawOptions(isRomConfig, menu_current);

            // restore shader
            if (GameLooping) {
                renderer->SetShader(shader);
            }

            Flip();
            renderer->Delay(INPUT_DELAY);
        }
    }

    if (GameLooping) {
        menu_current->childs.erase(menu_current->childs.end() - 3, menu_current->childs.end());
        for (int i = 0; i < 3; i++) {
            Clear();
            Flip();
        }
    }

    if (option_changed) {
        if (isRomConfig) {
            config->Save(romSelected);
        } else {
            config->Save();
        }
        UpdateInputMapping(isRomConfig);
    }

    if (stop) {
        GameLooping = false;
    }

    input->Clear(0);
}

void Gui::Run() {

    /*
    RomList::Rom r;
    char *name = (char *) malloc(4);
    strcpy(name, "dino");
    r.zip = name;
    RunRom(&r);
    */

#ifndef STANDARD_LAYOUT
    char sStr[MAX_PATH];
#endif

    Clear();
    DrawBg();
    DrawRomList();
    Flip();

    Timer *timer_input = new Timer();
    Timer *timer_load = new Timer();

    UpdateInputMapping(false);

    while (!quit) {

        int key = input->Update(0)[0].state;
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
#ifndef STANDARD_LAYOUT
            } else if (key & Input::Key::KEY_FIRE4) {
                if (romSelected != NULL) {
                    int hw_cfg = config->GetGuiValue(Option::Index::GUI_SHOW_HARDWARE);
                    RomList::Hardware hw = config->hardwareList[hw_cfg];
                    int show_clones = config->GetGuiValue(Option::Index::GUI_SHOW_CLONES);
                    int available = (int) roms.size();
                    if (config->GetGuiValue(Option::Index::GUI_SHOW_ALL)) {
                    	available = show_clones ? hw.available_count : hw.available_count - hw.available_clone_count;
                    }
                    sprintf(sStr, "%s\n\n%s.zip\n\nROMS: %i / %i", romSelected->name, romSelected->zip, available, (int) roms.size());
                    MessageBox(sStr, "Ok", NULL);
                    input->Clear(0);
                }
#endif
            } else if (key & Input::Key::KEY_MENU1) {
                RunOptionMenu();
                if (title != NULL) {
                    // refresh preview/title image
                    DrawBg();
                    DrawRomList();
#ifdef STANDARD_LAYOUT
                    title->Draw(GetRectRomPreview(), true);
#else
                    title->Draw(GetRectRomTitle(), true);
#endif
                    Flip();
                }
            } else if (key & Input::Key::KEY_MENU2) {
                if (romSelected != NULL) {
                    config->Load(romSelected);
                    RunOptionMenu(true);
                    if (title != NULL) {
                        // refresh preview/title image
                        DrawBg();
                        DrawRomList();
#ifdef STANDARD_LAYOUT
                        title->Draw(GetRectRomPreview(), true);
#else
                        title->Draw(GetRectRomTitle(), true);
#endif
                        Flip();
                    }
                }
            } else if (key & EV_QUIT) {
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

Gui::Gui(Io *i, Renderer *rdr, Skin *sk, RomList *rList, Config *cfg, Input *in) {

    io = i;
    renderer = rdr;
    skin = sk;
    romList = rList;
    config = cfg;
    input = in;

    skin->font->color = C2D_COL_WHITE;

    // build menus from options
    menu_gui = new Menu(NULL, cfg->GetGuiOptions());
    menu_rom = new Menu(NULL, cfg->GetRomOptions(), true);
    menu_current = menu_gui;

    // filter roms
    FilterRoms();
}

const Rect Gui::GetRectTitle() {

    Rect rect;

    rect.x = BORDER_SIZE;
    rect.y = BORDER_SIZE;
    rect.w = renderer->width / 2;
    rect.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rect.h = renderer->height / 4;
    rect.h -= BORDER_SIZE;

    return rect;
}

const Rect Gui::GetRectRomList() {

    Rect rect;

#ifdef STANDARD_LAYOUT
    rect.x = BORDER_SIZE;
    rect.y = renderer->height / 4;
    rect.y += BORDER_SIZE;
    rect.w = renderer->width / 2;
    rect.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rect.h = (renderer->height / 4) * 3;
    rect.h -= BORDER_SIZE * 2;
#else
    rect.x = BORDER_SIZE;
    rect.y = BORDER_SIZE;
    rect.w = renderer->width / 2;
    rect.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rect.h = renderer->height - (BORDER_SIZE * 2);
#endif
    return rect;
}

#ifdef STANDARD_LAYOUT
const Rect Gui::GetRectRomPreview() {
#else
const Rect Gui::GetRectRomTitle() {
#endif
    Rect rect;

    rect.x = (renderer->width / 2);
    rect.x += BORDER_SIZE / 2;
    rect.y = BORDER_SIZE;
    rect.w = (renderer->width / 2);
    rect.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rect.h = (renderer->height / 2);
    rect.h -= BORDER_SIZE + BORDER_SIZE / 2;

    return rect;
}

const Rect Gui::GetRectRomInfo() {

    Rect rect;

    rect.x = (renderer->width / 2);
    rect.x += BORDER_SIZE / 2;
    rect.y = (renderer->height / 2) + BORDER_SIZE / 2;
    rect.w = (renderer->width / 2);
    rect.w -= BORDER_SIZE + BORDER_SIZE / 2;
    rect.h = (renderer->height / 2);
    rect.h -= BORDER_SIZE + BORDER_SIZE / 2;

    return rect;
}

void Gui::TitleFree() {
    if (title) {
        delete (title);
        title = NULL;
    }

#ifndef STANDARD_LAYOUT
    if (preview) {
        delete (preview);
        preview = NULL;
    }
#endif
    title_loaded = 0;
}

int Gui::TitleLoad(RomList::Rom *rom) {

    TitleFree();

    char path[MAX_PATH];

#ifdef STANDARD_LAYOUT
    sprintf(path, "%s/%s.png", szAppPreviewPath, rom->zip);
    if (io->Exist(path)) {
        title = (Texture *) new C2DTexture(renderer, path);
        return title != NULL;
    } else if (rom->parent) {
        memset(path, 0, MAX_PATH);
        sprintf(path, "%s/%s.png", szAppPreviewPath, rom->parent);
        if (io->Exist(path)) {
            title = (Texture *) new C2DTexture(renderer, path);
            return title != NULL;
        }
    }
    return 0;
#else
    // Load title picture
    sprintf(path, "%s/%s.png", szAppTitlePath, rom->zip);
    if (io->Exist(path)) {
        title = (Texture *) new C2DTexture(renderer, path);
        //return title != NULL;
    } 
    else 
    if (rom->parent) {
        memset(path, 0, MAX_PATH);
        sprintf(path, "%s/%s.png", szAppTitlePath, rom->parent);
        if (io->Exist(path)) 
        {
            title = (Texture *) new C2DTexture(renderer, path);
            //return title != NULL;
        }
    }

    // Load preview picture
    sprintf(path, "%s/%s.png", szAppPreviewPath, rom->zip);
    if (io->Exist(path)) {
        preview = (Texture *) new C2DTexture(renderer, path);
    } 
    else 
    if (rom->parent) {
        memset(path, 0, MAX_PATH);
        sprintf(path, "%s/%s.png", szAppPreviewPath, rom->parent);
        if (io->Exist(path)) {
            preview = (Texture *) new C2DTexture(renderer, path);
        }
    }

    return ((title != NULL) || (preview != NULL));
#endif
}

void Gui::FilterRoms() {

    roms.clear();

    int showClone = config->GetGuiValue(Option::Index::GUI_SHOW_CLONES);
    int showAll = config->GetGuiValue(Option::Index::GUI_SHOW_ALL);
    int showHardwareCfg = config->GetGuiValue(Option::Index::GUI_SHOW_HARDWARE);
    int showHardware = config->hardwareList[showHardwareCfg].prefix;

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
            if (io->Exist(path))
                break;
        }
    }

    if (!io->Exist(path)) {
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

    // set per rom input scheme
    UpdateInputMapping(true);

    printf("RunRom: RunEmulator: start\n");
    RunEmulator(this, nBurnDrvActive);

    // set default input scheme
    UpdateInputMapping(false);

    printf("RunRom: RunEmulator: return\n");
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

int Gui::MessageBox(const char *message, const char *choice1, const char *choice2) {

    int first = 1;
    int index = 0;
    int max_choice = choice2 == NULL ? 1 : 2;

    Rect win{
            renderer->width / 4,
            renderer->height / 4,
            renderer->width / 2,
            renderer->height / 2
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

        int key = input->Update(0)[0].state;
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
            if (video) {
                video->Render();
            }

            renderer->DrawRect(win, C2D_COL_GRAY);
            renderer->DrawRect(win, C2D_COL_GREEN, false);

            Rect r_msg = win;
            r_msg.y -= skin->font->GetHeight(message);
            skin->font->Draw(r_msg, C2D_COL_WHITE, true, true, message);

            renderer->DrawRect(buttons[0], C2D_COL_GRAY_LIGHT);
            renderer->DrawRect(buttons[0], C2D_COL_BLACK, false);
            skin->font->Draw(buttons[0], C2D_COL_WHITE, true, true, choice1);

            if (max_choice > 1) {
                renderer->DrawRect(buttons[1], C2D_COL_GRAY_LIGHT);
                renderer->DrawRect(buttons[1], C2D_COL_BLACK, false);
                skin->font->Draw(buttons[1], C2D_COL_WHITE, true, true, choice2);
            }

            renderer->DrawRect(buttons[index], C2D_COL_ORANGE, false);

            Flip();
            renderer->Delay(INPUT_DELAY);
        }
    }

    input->Clear(0);

}

int Gui::GetButton() {

    Rect box{
            renderer->width / 4,
            renderer->height / 4,
            renderer->width / 2,
            renderer->height / 2
    };

    Timer *timer = new Timer();

    input->Clear(0);

    while (true) {

        int btn = input->GetButton(0);
        if (btn >= 0 || timer->GetSeconds() >= 3) {
            return btn;
        }

        renderer->Clear();
        renderer->DrawRect(box, C2D_COL_GRAY);
        renderer->DrawRect(box, C2D_COL_GREEN, false);
        skin->font->Draw(box, C2D_COL_WHITE, true, true, "PRESS A BUTTON (%i)", 3 - timer->GetSeconds());
        Flip();
    }
}

void Gui::UpdateInputMapping(bool isRomConfig) {

    if (isRomConfig) {
        input->SetKeyboardMapping(config->GetRomPlayerInputKeys(0));
        int dz = 2000 + config->GetRomValue(Option::Index::JOY_DEADZONE) * 2000;
        for (int i = 0; i < PLAYER_COUNT; i++) {
            input->SetJoystickMapping(i, config->GetRomPlayerInputButtons(i), dz);
            input->players[i].lx.id = config->GetRomValue(Option::Index::JOY_AXIS_LX);
            input->players[i].ly.id = config->GetRomValue(Option::Index::JOY_AXIS_LY);
            input->players[i].rx.id = config->GetRomValue(Option::Index::JOY_AXIS_RX);
            input->players[i].ry.id = config->GetRomValue(Option::Index::JOY_AXIS_RY);
        }
    } else {
        input->SetKeyboardMapping(config->GetGuiPlayerInputKeys(0));
        int dz = 2000 + config->GetGuiValue(Option::Index::JOY_DEADZONE) * 2000;
        for (int i = 0; i < PLAYER_COUNT; i++) {
            input->SetJoystickMapping(i, config->GetGuiPlayerInputButtons(i), dz);
            input->players[i].lx.id = config->GetGuiValue(Option::Index::JOY_AXIS_LX);
            input->players[i].ly.id = config->GetGuiValue(Option::Index::JOY_AXIS_LY);
            input->players[i].rx.id = config->GetGuiValue(Option::Index::JOY_AXIS_RX);
            input->players[i].ry.id = config->GetGuiValue(Option::Index::JOY_AXIS_RY);
        }
    }
}

void Gui::Clear() {
    renderer->Clear();
}

void Gui::Flip() {
    renderer->Flip();
}

Gui::~Gui() {
    delete (menu_gui);
    delete (menu_rom);
}
