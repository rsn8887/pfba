//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_PSP2_UTILITY_H
#define FBA_PSP2_UTILITY_H

#include <skeleton/utility.h>

class PSP2Utility : Utility {

public:

    bool FileExist(const char *file);

    std::vector<std::string> GetFileList(const char *path);
};

#endif //FBA_PSP2_UTILITY_H
