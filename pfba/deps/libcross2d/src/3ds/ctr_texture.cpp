//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include "ctr_texture.h"

CTRTexture::CTRTexture(const char *path) : Texture(path) {

    /*
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
    */
}

CTRTexture::CTRTexture(int w, int h) : Texture(w, h) {

    tex = sf2d_create_texture(w, h, TEXFMT_RGB565, SF2D_PLACE_VRAM);
    if (!tex) {
        printf("PSP2Texture: couldn't create texture\n");
        return;
    }
    width = w;
    height = h;
}

void CTRTexture::SetFiltering(int filter) {
}

CTRTexture::~CTRTexture() {

    if (tex != NULL) {
        sf2d_free_texture(tex);
        tex = NULL;
    }
}
