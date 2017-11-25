//
// Created by cpasjuste on 01/12/16.
//

#include <cstring>
#include <citro3d.h>

#include "ctr_texture.h"
#include "ctr_png.h"

#define TILE_FLAGS(fmt) \
    (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(fmt) | GX_TRANSFER_OUT_FORMAT(fmt) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

static u16 pow2(int w) {
    if (w == 0)
        return 0;
    u16 n = 2;
    while (w > n)
        n <<= 1;
    return n;
}

CTRTexture::CTRTexture(const char *path) : Texture(path) {

    fmt = GPU_RGBA8;
    bpp = 4;

    u8 *tmp = CTRPng::Load(&width, &height, path);
    if (!tmp) {
        printf("CTRTexture: couldn't create texture (CTRPng::Load)\n");
        return;
    }

    pixels = (u8 *) linearAlloc((size_t) (width * height * bpp));
    if (!pixels) {
        printf("CTRTexture: couldn't create texture (linearAlloc pixels)\n");
        linearFree(tmp);
        return;
    }

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ((u32 *) pixels)[i * width + j] = __builtin_bswap32(((u32 *) tmp)[i * width + j]);
        }
    }
    linearFree(tmp);

    bool res = C3D_TexInit(&tex, pow2(width), pow2(height), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = NULL;
        return;
    }

    Tile();
    SetFiltering(TEXTURE_FILTER_POINT);
}

CTRTexture::CTRTexture(int w, int h, GPU_TEXCOLOR format) : Texture(w, h) {

    width = w;
    height = h;
    fmt = format;
    bpp = fmt == GPU_RGBA8 ? 4 : 2;
    pixels = (u8 *) linearAlloc((size_t) (width * height * bpp));
    if (!pixels) {
        return;
    }

    bool res = C3D_TexInitVRAM(&tex, pow2(width), pow2(height), fmt);
    if (!res) {
        printf("CTRTexture: couldn't create texture (C3D_TexInit)\n");
        linearFree(pixels);
        pixels = NULL;
        return;
    }

    SetFiltering(TEXTURE_FILTER_POINT);
}

void CTRTexture::SetFiltering(int filter) {

    GPU_TEXTURE_FILTER_PARAM param =
            filter == TEXTURE_FILTER_POINT ? GPU_NEAREST : GPU_LINEAR;

    C3D_TexSetFilter(&tex, GPU_LINEAR, param);
}

void CTRTexture::Tile() {

    GSPGPU_FlushDataCache(pixels, (u32) (width * height * bpp));
    //GSPGPU_FlushDataCache(tex.data, tex.size);

    GX_TRANSFER_FORMAT format =
            fmt == GPU_RGB565 ? GX_TRANSFER_FMT_RGB565 : GX_TRANSFER_FMT_RGBA8;

    C3D_SafeDisplayTransfer(
            (u32 *) pixels,
            (u32) GX_BUFFER_DIM(width, height),
            (u32 *) tex.data,
            (u32) GX_BUFFER_DIM(tex.width, tex.height),
            (u32) TILE_FLAGS(format)
    );

    gspWaitForPPF();
}

CTRTexture::~CTRTexture() {

    C3D_TexDelete(&tex);

    if (pixels != NULL) {
        linearFree(pixels);
    }
}
