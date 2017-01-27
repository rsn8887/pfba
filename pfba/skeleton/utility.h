//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_UTILITY_H
#define FBA_UTILITY_H

#ifdef __PSP2__
#include "psp2/psp2_utility.h"
#else

#include <string>
#include <vector>

class Utility {

public:
    static bool FileExist(const char *file);

    static std::vector<std::string> GetFileList(const char *path);
};

#endif //__PSP2__
#endif //FBA_UTILITY_H
