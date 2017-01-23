//
// Created by cpasjuste on 23/01/17.
//

#ifndef __PSP2__
#include <sys/stat.h>
#include <dirent.h>
#endif

#include "utility.h"

bool Utility::FileExist(const char *file) {
#ifndef __PSP2__
    struct stat buf;
    return (stat(file, &buf) == 0);
#else
    return false;
#endif
}

std::vector<std::string> Utility::GetFileList(const char *path) {
#ifndef __PSP2__
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                files.push_back(std::string(ent->d_name));
            }
        }
        closedir(dir);
    }
    return files;
#else
    return std::vector<std::string>();
#endif
}