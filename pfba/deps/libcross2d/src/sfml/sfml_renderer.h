//
// Created by cpasjuste on 21/11/16.
//

#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include "skeleton/renderer.h"
#include "sfml_shaders.h"
#include <SFML/Graphics.hpp>

class SFMLRenderer : Renderer {

public:
    SFMLRenderer(int w, int h, const std::string &shaderPath);

    virtual ~SFMLRenderer();

    virtual Font *LoadFont(const char *path, int size);

    void DrawFont(Font *font, int x, int y, const char *fmt, ...);

    virtual Texture *CreateTexture(int w, int h);

    virtual Texture *LoadTexture(const char *file);

    virtual void DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation);

    virtual int LockTexture(Texture *texture, const Rect &rect, void **pixels, int *pitch);

    virtual void UnlockTexture(Texture *texture);

    virtual void DrawLine(int x1, int y1, int x2, int y2, const Color &color);

    virtual void DrawRect(const Rect &rect, const Color &color, bool fill = true);

    virtual void Clip(const Rect &rect);

    virtual const Rect GetWindowSize();

    virtual void Clear();

    virtual void Flip();

    virtual void Delay(unsigned int ms);

    virtual void SetShader(int index);

    sf::RenderWindow window;
    sf::VideoMode mode;
};

#endif //SFML_RENDERER_H
