//
// Created by cpasjuste on 09/12/16.
//

#include "renderer.h"

Renderer::Renderer() {
    color = BLACK;
}

Renderer::~Renderer() {
}

Rect Renderer::DrawTexture(Texture *texture, const Rect *rect, bool fit) {

    Rect scale;
    if (fit) {
        scale = {rect->x, rect->y, rect->w, rect->h};
        scale.w = (int) (((float) texture->width * ((float) rect->h) / (float) texture->height));
    } else {
        scale = {rect->x, rect->y, texture->width, texture->height};
    }

    if (scale.w > rect->w) {
        scale.h = (int) (((float) rect->w * ((float) texture->height) / (float) texture->width));
        scale.w = rect->w;
    }

    scale.x += (rect->w - scale.w) / 2;
    scale.y += (rect->h - scale.h) / 2;

    DrawTexture(texture, scale.x, scale.y, scale.w, scale.h, 0);

    return scale;
}

void Renderer::DrawTexture(Texture *texture, int x, int y, int w, int h) {
    DrawTexture(texture, x, y, w, h, 0);
}

void Renderer::DrawTexture(Texture *texture, int x, int y) {
    DrawTexture(texture, x, y, texture->width, texture->height);
}

void
Renderer::DrawFont(Font *font, Rect *dst, Color *color, bool centerX, bool centerY, const char *fmt, ...) {

    if (font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    Rect rect { dst->x, dst->y, dst->w, dst->h };

    if (centerY) {
        rect.y = dst->y + (dst->h / 2) - font->GetHeight(msg) / 2;
    }

    if (centerX) {
        rect.x = dst->x + (dst->w / 2) - font->GetWidth(msg) / 2;
    }

    // cut message "properly" instead of clip
    char *msg_new = msg;
    while (font->GetWidth(msg_new) > dst->w) {
        int len = strlen(msg_new);
        if (len == 0) {
            break;
        }
        msg_new[strlen(msg_new) - 1] = 0;
    }

    Color c = font->color;
    font->color = *color;
    DrawFont(font, rect.x, rect.y, msg_new);
    font->color = c;
}

void Renderer::DrawFont(Font *font, Rect *dst, Color *color, const char *fmt, ...) {

    if (font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    DrawFont(font, dst, color, false, false, msg);
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    DrawLine(x1, y1, x2, y2, &color);
}

void Renderer::DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool fill) {
    Rect rect{x, y, w, h};
    Color color{r, g, b, a};
    DrawRect(&rect, &color, fill);
}

void Renderer::DrawBorder(Rect *rect, Color *color, bool inside) {

    Rect r {rect->x, rect->y, rect->w, rect->h};

    if (inside) {
        r = {rect->x + 1, rect->y + 1, rect->w - 2, rect->h - 2};
    }/* else {
        r = {rect->x - 1, rect->y - 1, rect->w + 2, rect->h + 2};
    }*/

    DrawRect(&r, color, false);
}