//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include <sfil.h>
#include "ctr_texture.h"

CTRTexture::CTRTexture(const char *path) : Texture(path) {

    char *ext = strrchr(path, '.');
    if (ext) {
        if (strcasecmp(ext + 1, "jpg") == 0
            || strcasecmp(ext + 1, "jpeg") == 0) {
            tex = sfil_load_JPEG_file(path, SF2D_PLACE_RAM);
        } else if (strcasecmp(ext + 1, "png") == 0) {
            tex = sfil_load_PNG_file(path, SF2D_PLACE_RAM);
        } else if (strcasecmp(ext + 1, "bmp") == 0) {
            tex = sfil_load_BMP_file(path, SF2D_PLACE_RAM);
        }
    }

    if (!tex) {
        printf("PSP2Texture: could not create texture from `%s`\n", path);
        return;
    }

    width = tex->width;
    height = tex->height;
}

CTRTexture::CTRTexture(int w, int h) : Texture(w, h) {

    tex = sf2d_create_texture(w, h, TEXFMT_RGB565, SF2D_PLACE_VRAM);
    if (!tex) {
        printf("PSP2Texture: couldn't create texture\n");
        return;
    }

    width = w;
    height = h;
    pixels = (u8 *) linearAlloc(width * height * 2);
}

void CTRTexture::SetFiltering(int filter) {
}

void CTRTexture::Tile() {

    const u32 flags = (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |
                       GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGB565) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB565) |
                       GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

    GSPGPU_FlushDataCache(pixels, (u32) ((width * height) << 2));
    GSPGPU_FlushDataCache(tex->tex.data, tex->tex.size);

    C3D_SafeDisplayTransfer(
            (u32 *) pixels,
            GX_BUFFER_DIM(width, height),
            (u32 *) tex->tex.data,
            GX_BUFFER_DIM(width, height),
            flags
    );

    gspWaitForPPF();
}

CTRTexture::~CTRTexture() {

    if (tex != NULL) {
        sf2d_free_texture(tex);
        tex = NULL;
    }

    if (pixels != NULL) {
        linearFree(pixels);
    }
}
