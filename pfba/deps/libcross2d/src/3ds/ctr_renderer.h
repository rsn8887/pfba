//
// Created by cpasjuste on 21/11/16.
//

#ifndef _CTR_RENDERER_H_
#define _CTR_RENDERER_H_

#include <skeleton/renderer.h>

extern void ctr_printf(const char *msg, ...);
#define printf ctr_printf

class CTRRenderer : Renderer {

public:
    CTRRenderer();
    ~CTRRenderer();

    Font *LoadFont(const char *path, int size);
    void DrawFont(Font *font, int x, int y, const char *fmt, ...);

    Texture *CreateTexture(int w, int h);
    Texture *LoadTexture(const char *file);
    void DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation);
    int LockTexture(Texture * texture, const Rect &rect, void **pixels, int *pitch);
    
    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);
    void DrawRect(const Rect &rect, const Color &color, bool fill = true);

    void Clip(const Rect &rect);

    const Rect GetWindowSize();

    void Clear();
    void Flip();
    
    void Delay(unsigned int ms);

    void SetShader(int shader);

private:
    void StartDrawing();
    bool drawing_started = false;
};

#endif //_CTR_RENDERER_H_
