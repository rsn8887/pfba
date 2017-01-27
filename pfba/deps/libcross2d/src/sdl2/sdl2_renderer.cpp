//
// Created by cpasjuste on 21/11/16.
//

#include <skeleton/renderer.h>

//////////
// INIT //
//////////
SDL2Renderer::SDL2Renderer(int w, int h) : Renderer() {

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        printf("Couldn't init sdl: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!w || !h) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
            "SDL2 GUI",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h, flags);

    if (window == NULL) {
        printf("Couldn't create window: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return;
    }

    this->shaders = new Shaders("");
}
//////////
// INIT //
//////////

//////////
// FONT //
//////////
Font *SDL2Renderer::LoadFont(const char *path, int size) {
    if (renderer == NULL) {
        return NULL;
    }
    Font *font = (Font *) new SDL2Font(renderer, path, size);
    return font;
}

void SDL2Renderer::DrawFont(Font *font, int x, int y, const char *fmt, ...) {

    if (font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    SDL_Color c{font->color.r, font->color.g, font->color.b, font->color.a};
    FC_DrawColor(((SDL2Font *) font)->font, renderer, x, y, c, msg);
}

//////////
// FONT //
//////////

/////////////
// TEXTURE //
/////////////
Texture *SDL2Renderer::CreateTexture(int w, int h) {
    if (renderer == NULL) {
        return NULL;
    }

    SDL2Texture *texture = new SDL2Texture(renderer, w, h);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

Texture *SDL2Renderer::LoadTexture(const char *file) {
    if (renderer == NULL) {
        return NULL;
    }

    SDL2Texture *texture = new SDL2Texture(renderer, file);
    if (texture->tex == NULL) {
        delete (texture);
        return NULL;
    }
    return (Texture *) texture;
}

void SDL2Renderer::DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation) {
    SDL_Rect rect{x, y, w, h};
    SDL_RenderCopyEx(renderer, ((SDL2Texture *) texture)->tex, NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
}

int SDL2Renderer::LockTexture(Texture *texture, const Rect &rect, void **pixels, int *pitch) {
    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
        return SDL_LockTexture(((SDL2Texture *) texture)->tex, &r, pixels, pitch);
    } else {
        return SDL_LockTexture(((SDL2Texture *) texture)->tex, NULL, pixels, pitch);
    }
}

void SDL2Renderer::UnlockTexture(Texture *texture) {
    SDL_UnlockTexture(((SDL2Texture *) texture)->tex);
}
/////////////
// TEXTURE //
/////////////

const Rect SDL2Renderer::GetWindowSize() {
    Rect rect;
    SDL_GetWindowSize(window, &rect.w, &rect.h);
    return rect;
}

void SDL2Renderer::SetShader(int shader) {
    // TODO
}

void SDL2Renderer::DrawLine(int x1, int y1, int x2, int y2, const Color &c) {

    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDL2Renderer::DrawRect(const Rect &rect, const Color &c, bool fill) {

    SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    if (fill) {
        SDL_RenderFillRect(renderer, &r);
    } else {
        SDL_RenderDrawRect(renderer, &r);
    }
    if (c.a < 255) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDL2Renderer::Clip(const Rect &rect) {

    if (rect.x != 0 || rect.y != 0 || rect.w != 0 || rect.h != 0) {
        SDL_Rect r{rect.x, rect.y, rect.w, rect.h};
        SDL_RenderSetClipRect(renderer, &r);
    } else {
        SDL_RenderSetClipRect(renderer, NULL);
    }
}

void SDL2Renderer::Clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::Flip() {
    SDL_RenderPresent(renderer);
}

void SDL2Renderer::Delay(unsigned int ms) {
    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
