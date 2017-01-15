//
// Created by cpasjuste on 21/11/16.
//

#include <SDL2/SDL.h>
#include <psp2_renderer.h>
#include "psp2_font.h"
#include "psp2_texture.h"

#include "lcd3x_v.h"
#include "lcd3x_f.h"
#include "gtu_v.h"
#include "gtu_f.h"
#include "texture_v.h"
#include "texture_f.h"
#include "opaque_v.h"
#include "bicubic_f.h"
#include "xbr_2x_v.h"
#include "xbr_2x_f.h"
#include "xbr_2x_fast_v.h"
#include "xbr_2x_fast_f.h"
#include "advanced_aa_v.h"
#include "advanced_aa_f.h"
#include "scale2x_f.h"
#include "scale2x_v.h"
#include "sharp_bilinear_f.h"
#include "sharp_bilinear_v.h"
#include "sharp_bilinear_simple_f.h"
#include "sharp_bilinear_simple_v.h"
//#include "xbr_2x_noblend_f.h"
//#include "xbr_2x_noblend_v.h"
#include "fxaa_v.h"
#include "fxaa_f.h"
#include "crt_easymode_f.h"

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

    shaderCount = 8;
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

    int height = vita2d_pgf_text_height(((PSP2Font *) font)->font, 1, msg) - 2; // fixme (-3)

    StartDrawring();
    vita2d_pgf_draw_text(((PSP2Font *) font)->font, x, y + height,
                         (unsigned int) RGBA8(font->color.r, font->color.g, font->color.b, font->color.a), 1, msg);
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
    if(texture && ((PSP2Texture *) texture)->tex) {

        float sx = (float) w / (float) texture->width;
        float sy = (float) h / (float) texture->height;
        StartDrawring();

        const float rad = rotation * 0.0174532925f;
        vita2d_draw_texture_scale_rotate(((PSP2Texture *) texture)->tex,
                                         x + w / 2, y + h / 2,
                                         sx, sy, rad);
    }
}

int PSP2Renderer::LockTexture(Texture *texture, const Rect *rect, void **pixels, int *pitch) {
    *pixels = vita2d_texture_get_datap(((PSP2Texture *) texture)->tex);
    *pitch = vita2d_texture_get_stride(((PSP2Texture *) texture)->tex);
    return 0;
}

/////////////
// TEXTURE //
/////////////

Rect PSP2Renderer::GetWindowSize() {
    Rect rect{0, 0, 960, 544};
    return rect;
}

void PSP2Renderer::SetShader(int shaderType) {

    if (shader != NULL) {
        vita2d_wait_rendering_done();
        vita2d_free_shader(shader);
    }

    switch (shaderType) {

        case ShaderType::lcd3x:
            shader = vita2d_create_shader((SceGxmProgram *) lcd3x_v, (SceGxmProgram *) lcd3x_f);
            break;
        case ShaderType::scale2x:
            shader = vita2d_create_shader((SceGxmProgram *) scale2x_v, (SceGxmProgram *) scale2x_f);
            break;
        case ShaderType::advanced_aa:
            shader = vita2d_create_shader((SceGxmProgram *) advanced_aa_v, (SceGxmProgram *) advanced_aa_f);
            break;
        case ShaderType::sharp_bilinear:
            shader = vita2d_create_shader((SceGxmProgram *) sharp_bilinear_simple_v, (SceGxmProgram *) sharp_bilinear_simple_f);
            break;
        case ShaderType::sharp_bilinear_scanlines:
            shader = vita2d_create_shader((SceGxmProgram *) sharp_bilinear_v, (SceGxmProgram *) sharp_bilinear_f);
            break;
        case ShaderType::fxaa:
            shader = vita2d_create_shader((SceGxmProgram *) fxaa_v, (SceGxmProgram *) fxaa_f);
            break;
        default:
            shader = vita2d_create_shader((SceGxmProgram *) texture_v, (SceGxmProgram *) texture_f);
            break;
    }

    vita2d_texture_set_program(shader->vertexProgram, shader->fragmentProgram);
    vita2d_texture_set_wvp(shader->wvpParam);
    vita2d_texture_set_vertexInput(&shader->vertexInput);
    vita2d_texture_set_fragmentInput(&shader->fragmentInput);

    shaderCurrent = shaderType;
}

void PSP2Renderer::DrawLine(int x1, int y1, int x2, int y2, Color *_color) {
    StartDrawring();
    vita2d_draw_line(x1, y1, x2, y2,
                     RGBA8(_color->r, _color->g, _color->b, _color->a));
}

void PSP2Renderer::DrawRect(Rect *rect, Color *_color, bool fill) {
    StartDrawring();
    if (fill) {
        vita2d_draw_rectangle(rect->x, rect->y, rect->w, rect->h,
                              RGBA8(_color->r, _color->g, _color->b, _color->a));
    } else {
        DrawLine(rect->x, rect->y + 1, rect->x + rect->w, rect->y + 1, _color);               // top
        DrawLine(rect->x + 1, rect->y, rect->x + 1, rect->y + rect->h, _color);               // left
        DrawLine(rect->x, rect->y + rect->h, rect->x + rect->w, rect->y + rect->h, _color);   // bottom
        DrawLine(rect->x + rect->w, rect->y, rect->x + rect->w, rect->y + rect->h, _color);   // right
    }
}

void PSP2Renderer::Clip(Rect *rect) {

    if (rect) {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE,
                               rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
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
        //if (shaderCurrent != ShaderType::texture) // screen tearing on some games
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
    if (shader != NULL) {
        vita2d_free_shader(shader);
    }
}

void PSP2Renderer::StartDrawring() {
    if (!drawing_started) {
        vita2d_start_drawing();
        drawing_started = true;
    }
}