//
// Created by cpasjuste on 16/02/17.
//

#ifndef PFBA_MENU_H
#define PFBA_MENU_H

#include <vector>
#include "option.h"

class Menu {

public:

    Menu(Menu *parent, std::vector<Option> *options, bool isRomCfg = false);
    ~Menu();

    void AddChild(const std::string &title);

    std::string title;
    std::vector<Menu*> childs;
    Menu *parent = NULL;
    std::vector<int> option_ids;

private:

};


#endif //PFBA_MENU_H
