//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_SDL2UTILITY_H
#define FBA_SDL2UTILITY_H

#include <skeleton/utility.h>

class SDL2Utility : Utility {

public:
    virtual bool FileExist(const char *file);

    virtual std::vector<std::string> GetFileList(const char *path);
};


#endif //FBA_SDL2UTILITY_H
