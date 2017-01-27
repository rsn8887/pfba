//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_PSP2_UTILITY_H
#define FBA_PSP2_UTILITY_H

#include <string>
#include <vector>

class Utility {

public:
    static bool FileExist(const char *file);

    static std::vector<std::string> GetFileList(const char *path);

};

#endif //FBA_PSP2_UTILITY_H
