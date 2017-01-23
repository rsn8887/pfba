//
// Created by cpasjuste on 21/11/16.
//

#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include "skeleton/renderer.h"
#include <SFML/Graphics.hpp>

class SFMLRenderer : Renderer {

public:
    SFMLRenderer(int w, int h);
    virtual ~SFMLRenderer();

    virtual Font *LoadFont(const char *path, int size);
    void DrawFont(Font *font, int x, int y, const char *fmt, ...);

    virtual Texture *CreateTexture(int w, int h);
    virtual Texture *LoadTexture(const char *file);
    virtual void DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation);
    virtual int LockTexture(Texture * texture, const Rect * rect, void **pixels, int *pitch);
    virtual void UnlockTexture(Texture * texture);

    virtual void DrawLine(int x1, int y1, int x2, int y2, Color *color);
    virtual void DrawRect(Rect *rect, Color *color, bool fill = true);

    virtual void Clip(Rect *rect);

    virtual Rect GetWindowSize();

    virtual void Clear();
    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int shader);

    enum ShaderType {
        texture = 0,
        sharp_bilinear,
        sharp_bilinear_scanlines,
        lcd3x,
        advanced_aa,
        scale2x,
        end
    };

    sf::Shader shaders[ShaderType::end-1];
    sf::RenderWindow window;
};

#endif //SFML_RENDERER_H
