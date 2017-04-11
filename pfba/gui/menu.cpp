//
// Created by cpasjuste on 16/02/17.
//

#include "menu.h"

Menu::Menu(Menu *parent, std::vector<Option> *options, bool isRomCfg) {

    this->parent = parent;

    if (options == NULL) {
        return;
    }

    Menu *menu = this;

    for (int i = 0; i < options->size(); i++) {

        Option option = options->at(i);

        if (option.flags & Option::Type::HIDDEN
            || option.index == Option::Index::END) {
            continue;
        }

        if (option.flags & Option::Type::MENU) {
            if (i == 0 && !isRomCfg) {
                menu->title = option.GetName();
            } else {
                menu = new Menu(this, NULL);
                menu->title = option.GetName();
                childs.push_back(menu);
            }
        } else {
            menu->option_ids.push_back(option.index);
        }
    }
}

void Menu::AddChild(const std::string &title) {
    Menu *m = new Menu(NULL, NULL);
    m->title = title;
    childs.push_back(m);
}

Menu::~Menu() {
    for (int i = 0; i < childs.size(); i++) {
        delete (childs[i]);
    }
    childs.clear();
    option_ids.clear();
}
