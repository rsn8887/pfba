//
// Created by cpasjuste on 24/11/17.
//

#ifndef PFBA_CTR_PNG_H
#define PFBA_CTR_PNG_H

#include "ctr_texture.h"

class CTRPng {

public:
    static u8* Load(int *width, int *height, const char *filename);

    static u8* Load(int *width, int *height, const void *buffer);
};

#endif //PFBA_CTR_PNG_H
