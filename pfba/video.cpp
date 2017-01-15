//
// Created by cpasjuste on 01/12/16.
//

#include <burner.h>
#include <gui/config.h>
#include "video.h"

extern Gui *gui;

static unsigned int myHighCol16(int r, int g, int b, int /* i */) {
    unsigned int t;
    t = (unsigned int) ((r << 8) & 0xf800); // rrrr r000 0000 0000
    t |= (g << 3) & 0x07e0; // 0000 0ggg ggg0 0000
    t |= (b >> 3) & 0x001f; // 0000 0000 000b bbbb
    return t;
}

Video::Video(Renderer *renderer) {

    this->renderer = renderer;

    BurnDrvGetFullSize(&VideoBufferWidth, &VideoBufferHeight);
    printf("game resolution: %ix%i\n", VideoBufferWidth, VideoBufferHeight);

    if(BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        printf("game orientation: vertical\n");
    }
    if(BurnDrvGetFlags() & BDF_ORIENTATION_FLIPPED) {
        printf("game orientation: flipped\n");
    }

    if (screen == NULL) {
        screen = renderer->CreateTexture(VideoBufferWidth, VideoBufferHeight);
    }

    nBurnBpp = 2;
    BurnHighCol = myHighCol16;
    BurnRecalcPal();
    renderer->LockTexture(screen, NULL, (void **) &pBurnDraw, &nBurnPitch);
    renderer->UnlockTexture(screen);

    renderer->SetShader(gui->GetConfig()->GetRomValue(Option::Index::ROM_SHADER));
    screen->SetFiltering(gui->GetConfig()->GetRomValue(Option::Index::ROM_FILTER));
    Scale();
}

void Video::Scale() {

    Rect window = renderer->GetWindowSize();
    printf("window: %ix%i\n", window.w, window.h);

    int scaling = gui->GetConfig()->GetRomValue(Option::Index::ROM_SCALING);
    rotation = 0;

    // TODO: force right to left orientation on psp2,
    // should add platform specific code
    if(!gui->GetConfig()->GetRomValue(Option::Index::ROM_ROTATION)
                  && BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        if(!(BurnDrvGetFlags() & BDF_ORIENTATION_FLIPPED)) {
            rotation = 180;
        }
    } else {
        if(BurnDrvGetFlags() & BDF_ORIENTATION_FLIPPED) {
            rotation = 90;
        } else if(BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
            rotation = -90;
        } else {
            rotation = 0;
        }
    }
    // TODO: force right to left orientation on psp2,
    // should add platform specific code

    printf("rotation: %i\n", rotation);

    scale.w = VideoBufferWidth;
    scale.h = VideoBufferHeight;

    switch (scaling) {

        case 1: // 2x
            scale.w = scale.w * 2;
            scale.h = scale.h * 2;
            break;

        case 2: // fit
            if(rotation == 0 || rotation == 180) {
                scale.h = window.h;
                scale.w = (int) ((float) scale.w *((float) scale.h / (float) VideoBufferHeight));
                if(scale.w > window.w) {
                    scale.w = window.w;
                    scale.h = (int) ((float) scale.w * ((float) VideoBufferHeight / (float) VideoBufferWidth));
                }
            } else {
                scale.w = window.h;
                scale.h = (int) ((float) scale.w * ((float) VideoBufferHeight / (float) VideoBufferWidth));
            }
            break;

        case 3: // fullscreen
            if(rotation == 0 || rotation == 180) {
                scale.h = window.h;
                scale.w = window.w;
            } else {
                scale.h = window.w;
                scale.w = window.h;
            }
            break;

        default:
            break;
    }

    scale.x = (window.w - scale.w) / 2;
    scale.y = (window.h - scale.h) / 2;

    printf("scale: %ix%i\n", scale.w, scale.h);

    for(int i=0; i<3; i++) {
        Clear();
        Flip();
    }
}

void Video::Clear() {
    renderer->Clear();
}

void Video::Lock() {
    renderer->LockTexture(screen, NULL, (void **) &pBurnDraw, &nBurnPitch);
}

void Video::Unlock() {
    renderer->UnlockTexture(screen);
}

void Video::Render() {
    if(pBurnDraw != NULL) {
        renderer->DrawTexture(screen, scale.x, scale.y, scale.w, scale.h, rotation);
    }
}

void Video::Flip() {
    renderer->Flip();
}

Video::~Video() {
    if (screen != NULL) {
        delete (screen);
        screen = NULL;
    }
    pBurnDraw = NULL;
    renderer->SetShader(0);
}
