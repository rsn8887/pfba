//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include "psp2_texture.h"

#ifdef __PSP2_DEBUG__
#include <psp2shell.h>
#define printf psp2shell_print
#else
#define printf
#endif

PSP2Texture::PSP2Texture(const char *path) : Texture(path) {

    char *ext = strrchr(path, '.');
    if (ext) {
        if (strcasecmp(ext + 1, "jpg") == 0
            || strcasecmp(ext + 1, "jpeg") == 0) {
            tex = vita2d_load_JPEG_file(path);
        } else if (strcasecmp(ext + 1, "png") == 0) {
            tex = vita2d_load_PNG_file(path);
        } else if (strcasecmp(ext + 1, "bmp") == 0) {
            tex = vita2d_load_BMP_file(path);
        }
    }

    if (!tex) {
        printf("PSP2Texture: could not create texture from `%s`\n", path);
        return;
    }
    width = vita2d_texture_get_width(tex);
    height = vita2d_texture_get_height(tex);
}

PSP2Texture::PSP2Texture(int w, int h) : Texture(w, h) {

    tex = vita2d_create_empty_texture_format(
            (unsigned int) w, (unsigned int) h, SCE_GXM_TEXTURE_FORMAT_R5G6B5);
    if (!tex) {
        printf("PSP2Texture: couldn't create texture\n");
        return;
    }
    width = vita2d_texture_get_width(tex);
    height = vita2d_texture_get_height(tex);
}

void PSP2Texture::SetFiltering(int filter) {

    SceGxmTextureFilter tf = SCE_GXM_TEXTURE_FILTER_POINT;

    switch (filter) {
        case TEXTURE_FILTER_LINEAR:
            tf = SCE_GXM_TEXTURE_FILTER_LINEAR;
            break;

        case TEXTURE_FILTER_POINT_ANISO:
            tf = SCE_GXM_TEXTURE_FILTER_ANISO_POINT;
            break;

        case TEXTURE_FILTER_LINEAR_ANISO:
            tf = SCE_GXM_TEXTURE_FILTER_ANISO_LINEAR;
            break;

        case TEXTURE_FILTER_POINT:
        default:
            break;
    }

    vita2d_texture_set_filters(tex, SCE_GXM_TEXTURE_FILTER_POINT, tf);
}

PSP2Texture::~PSP2Texture() {

    if (tex != NULL) {
        vita2d_wait_rendering_done();
        vita2d_free_texture(tex);
        tex = NULL;
    }
}
