//
// Created by cpasjuste on 19/12/16.
//

#ifndef FBA_UTILITY_H
#define FBA_UTILITY_H

#include <string>
#include <vector>

class Utility {

public:
    virtual bool FileExist(const char *file) {
        printf("Utility::FileExist: not implemented\n");
        return false;
    };

    virtual std::vector<std::string> GetFileList(const char *path) {
        printf("Utility::GetFileList: not implemented\n");
        return std::vector<std::string>();
    };
};

#endif //FBA_UTILITY_H
