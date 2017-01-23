//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_UTILITY_H
#define FBA_UTILITY_H

#include <string>
#include <vector>

class Utility {

public:
    virtual bool FileExist(const char *file);
    virtual std::vector<std::string> GetFileList(const char *path);
};

#endif //FBA_UTILITY_H
