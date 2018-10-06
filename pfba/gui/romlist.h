//
// Created by cpasjuste on 19/10/16.
//

#ifndef _ROMLIST_H
#define _ROMLIST_H

#include <vector>

#include "burner.h"
#define HARDWARE_PREFIX_ALL 0xffffffff

#include "skin.h"

class RomList {

public:

    struct Hardware {
        int prefix;
        std::string name;
        int clone_count = 0;
        int supported_count = 0;
        int available_count = 0;
        int available_clone_count = 0;
        int missing_count = 0;
        int missing_clone_count = 0;
        Hardware(int p, const std::string &n, int rs = 0, int ra = 0) {
            prefix = p;
            name = n;
            supported_count = rs;
            available_count = 0;
        }
    };

    RomList(Io *io, std::vector<Hardware> *hwList,
            const std::vector<std::string> &paths, Renderer *rdr, Skin *skin);
    ~RomList();

    void DrawSplashScreen(Renderer *rdr, Skin *skin);

    enum RomState {
        NOT_WORKING = 0,
        MISSING,
        WORKING
    };

    struct Rom {
        char *name;
        char *zip;
        char *year;
        char *manufacturer;
        char *parent;
        int flags;
        int state;
        int hardware;
        char *system;
        int genre;
        int size;
    };

    std::vector<Rom> list;
    std::vector<Hardware> *hardwareList;

    Hardware *GetHardware(int hardware_prefix) {
        for (int i = 0; i < hardwareList->size(); i++) {
            if (IsHardware(hardware_prefix, hardwareList->at(i).prefix)) {
                return &hardwareList->at(i);
            }
        }
        return NULL;
    };

    const char *GetHardwareName(int hardware_prefix) {
        for (int i = 0; i < hardwareList->size(); i++) {
            if (hardwareList->at(i).prefix == hardware_prefix) {
                return hardwareList->at(i).name.c_str();
            }
        }
        return "UNKNOW";
    };

    static bool IsHardware(int hardware, int type) {
        return (((hardware | HARDWARE_PREFIX_CARTRIDGE) ^ HARDWARE_PREFIX_CARTRIDGE)
                & 0xff000000) == type;
    }
};

#endif //_ROMLIST_H
