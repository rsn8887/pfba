//
// Created by cpasjuste on 01/12/16.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#define TEXTURE_FILTER_POINT 0
#define TEXTURE_FILTER_LINEAR 1

#include <cstdio>

class Texture {

public:
    Texture(const char *path) {};
    Texture(int w, int h) {};
    virtual ~Texture() {};

    virtual void SetFiltering(int filter) {
        printf("Texture::SetFiltering: not implemented\n");
    };

    int width = 0;
    int height = 0;
};

#endif //_TEXTURE_H_
