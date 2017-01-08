//
// Created by cpasjuste on 19/12/16.
//

#include <sys/stat.h>
#include <dirent.h>
#include "sdl2_utility.h"

bool SDL2Utility::FileExist(const char *file) {

    struct stat buf;
    return (stat(file, &buf) == 0);
}

std::vector<std::string> SDL2Utility::GetFileList(const char *path) {

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
}
