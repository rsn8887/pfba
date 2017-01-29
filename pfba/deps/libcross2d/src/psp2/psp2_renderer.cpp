//
// Created by cpasjuste on 21/11/16.
//

#include <SDL2/SDL.h>
#include <psp2_renderer.h>
#include "psp2_font.h"
#include "psp2_texture.h"
#include "psp2_shaders.h"

//////////
// INIT //
//////////
PSP2Renderer::PSP2Renderer(int w, int h) : Renderer() {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    vita2d_init();
    vita2d_set_clear_color((unsigned int) RGBA8(color.r, color.g, color.b, color.a));

    this->shaders = (Shaders *) new PSP2Shaders("");
    SetShader(0);
}
//////////
// INIT //
//////////

//////////
// FONT //
//////////
Font *PSP2Renderer::LoadFont(const char *path, int size) {
    Font *font = (Font *)
            new PSP2Font(path, size);
    return font;
}

void PSP2Renderer::DrawFont(Font *font, int x, int y, const char *fmt, ...) {

    if (font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    int height = vita2d_pgf_text_height(((PSP2Font *) font)->font, font->scaling, msg) - 2; // fixme (-2)

    StartDrawring();
    vita2d_pgf_draw_text(((PSP2Font *) font)->font, x, y + height*2,
                         (unsigned int) RGBA8(font->color.r, font->color.g,
                                              font->color.b, font->color.a),
                         font->scaling, msg);
}
//////////
// FONT //
//////////

/////////////
// TEXTURE //
/////////////
Texture *PSP2Renderer::CreateTexture(int w, int h) {
    PSP2Texture *texture = new PSP2Texture(w, h);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

Texture *PSP2Renderer::LoadTexture(const char *file) {
    PSP2Texture *texture = new PSP2Texture(file);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

void PSP2Renderer::DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation) {
    if (texture && ((PSP2Texture *) texture)->tex) {

        float sx = (float) w / (float) texture->width;
        float sy = (float) h / (float) texture->height;
        StartDrawring();

        const float rad = rotation * 0.0174532925f;
        vita2d_draw_texture_scale_rotate(((PSP2Texture *) texture)->tex,
                                         x + w / 2, y + h / 2,
                                         sx, sy, rad);
    }
}

int PSP2Renderer::LockTexture(Texture *texture, const Rect &rect, void **pixels, int *pitch) {
    *pixels = vita2d_texture_get_datap(((PSP2Texture *) texture)->tex);
    *pitch = vita2d_texture_get_stride(((PSP2Texture *) texture)->tex);
    return 0;
}

/////////////
// TEXTURE //
/////////////

const Rect PSP2Renderer::GetWindowSize() {
    Rect rect{0, 0, 960, 544};
    return rect;
}

void PSP2Renderer::SetShader(int index) {

    if (index == shaders->current || index >= shaders->Count()) {
        return;
    }
    shaders->current = index;

    vita2d_shader *shader = (vita2d_shader *) shaders->Get()->data;
    if (shader != NULL) {
        vita2d_texture_set_program(shader->vertexProgram, shader->fragmentProgram);
        vita2d_texture_set_wvp(shader->wvpParam);
        vita2d_texture_set_vertexInput(&shader->vertexInput);
        vita2d_texture_set_fragmentInput(&shader->fragmentInput);
    }
}

void PSP2Renderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {
    StartDrawring();
    vita2d_draw_line(x1, y1, x2, y2,
                     RGBA8(c.r, c.g, c.b, c.a));
}

void PSP2Renderer::DrawRect(const Rect &rect, const Color &c, bool fill) {
    StartDrawring();
    if (fill) {
        vita2d_draw_rectangle(rect.x, rect.y, rect.w, rect.h,
                              RGBA8(c.r, c.g, c.b, c.a));
    } else {
        DrawLine(rect.x, rect.y + 1, rect.x + rect.w, rect.y + 1, c);               // top
        DrawLine(rect.x + 1, rect.y, rect.x + 1, rect.y + rect.h, c);               // left
        DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, c);   // bottom
        DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, c);   // right
    }
}

void PSP2Renderer::Clip(const Rect &rect) {

    // vita2d_set_region_clip doesn't work correctly
    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE,
                               rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
    } else {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_NONE, 0, 0, 0, 0);
    }
}

void PSP2Renderer::Clear() {
    StartDrawring();
    vita2d_clear_screen();
}

void PSP2Renderer::Flip() {
    if (drawing_started) {
        vita2d_end_drawing();
        vita2d_wait_rendering_done();
        vita2d_swap_buffers();
        drawing_started = false;
    }
}

void PSP2Renderer::Delay(unsigned int ms) {
    SDL_Delay(ms);
}

PSP2Renderer::~PSP2Renderer() {
    vita2d_wait_rendering_done();
    vita2d_fini();
    delete (shaders);
}

void PSP2Renderer::StartDrawring() {
    if (!drawing_started) {
        vita2d_start_drawing();
        drawing_started = true;
    }
}