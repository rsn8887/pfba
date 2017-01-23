//
// Created by cpasjuste on 21/11/16.
//

#include "sfml_renderer.h"
#include "sfml_font.h"
#include "GL/gl.h"
#include "sfml_texture.h"

//////////
// INIT //
//////////
SFMLRenderer::SFMLRenderer(int w, int h) : Renderer() {

    window.create(sf::VideoMode(w, h), "LIBCROSS2D");
    window.setVerticalSyncEnabled(true);

    if (sf::Shader::isAvailable()) {
        //shaders[ShaderType::lcd3x].loadFromMemory(lcd3x_v, lcd3x_f);
        //shaders[ShaderType::scale2x].loadFromMemory(scale2x_v, scale2x_f);
        //shaders[ShaderType::advanced_aa].loadFromMemory(advanced_aa_v, advanced_aa_f);
        //shaders[ShaderType::sharp_bilinear].loadFromMemory(sharp_bilinear_simple_v, sharp_bilinear_simple_f);
        //shaders[ShaderType::sharp_bilinear_scanlines].loadFromMemory(sharp_bilinear_v, sharp_bilinear_f);
        //shaders[ShaderType::texture].loadFromMemory(texture_v, texture_f);
        //shaders[ShaderType::texture].loadFromFile("s.v", "s.f");
    }
}
//////////
// INIT //
//////////

//////////
// FONT //
//////////
Font *SFMLRenderer::LoadFont(const char *path, int size) {
    Font *font = (Font *) new SFMLFont(path, size);
    return font;
}

void SFMLRenderer::DrawFont(Font *font, int x, int y, const char *fmt, ...) {
    if (font == NULL) {
        return;
    }

    char msg[MAX_PATH];
    memset(msg, 0, MAX_PATH);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, MAX_PATH, fmt, args);
    va_end(args);

    SFMLFont *fnt = (SFMLFont *) font;
    sf::Text text;
    text.setFont(fnt->font);
    text.setString(msg);
    text.setCharacterSize(font->size);
    text.setFillColor(sf::Color(
            font->color.r, font->color.g,
            font->color.b, font->color.a));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setPosition(x, y);
    window.draw(text);
}

//////////
// FONT //
//////////

/////////////
// TEXTURE //
/////////////
Texture *SFMLRenderer::CreateTexture(int w, int h) {
    SFMLTexture *texture = new SFMLTexture(w, h);
    if (!texture->pixels) {
        return NULL;
    }
    return (Texture *) texture;
}

Texture *SFMLRenderer::LoadTexture(const char *file) {
    SFMLTexture *texture = new SFMLTexture(file);
    if (!texture->pixels) {
        return NULL;
    }
    return (Texture *) texture;
}

void SFMLRenderer::DrawTexture(Texture *texture, int x, int y, int w, int h, float rotation) {
    sf::Sprite sprite = ((SFMLTexture *) texture)->sprite;
    sprite.setPosition(x, y);

    float scaleX = (float) w / (float) texture->width;
    float scaleY = (float) h / (float) texture->height;
    sprite.setScale(scaleX, scaleY);

    sf::Transform transform;
    transform.rotate(rotation, {x + w / 2, y + h / 2});
    //shaders[ShaderType::texture].setUniform("rubyTexture", sf::Shader::CurrentTexture);
    //shaders[ShaderType::texture].setUniform("rubyTextureSize", sf::Glsl::Vec2(w, h));

    sf::RenderStates states;
    //states.shader = &shaders[ShaderType::texture];
    states.transform = transform;
    window.draw(sprite, states);
}

int SFMLRenderer::LockTexture(Texture *texture, const Rect *rect, void **pixels, int *pitch) {
    *pixels = ((SFMLTexture *) texture)->pixels;
    *pitch = texture->width * 2;
    return 0;
}

void SFMLRenderer::UnlockTexture(Texture *texture) {
    GLint textureBinding;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
    sf::Texture::bind(&((SFMLTexture *) texture)->texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height,
                    GL_RGB, GL_UNSIGNED_SHORT_5_6_5, ((SFMLTexture *) texture)->pixels);
    glBindTexture(GL_TEXTURE_2D, textureBinding);
}

/////////////
// TEXTURE //
/////////////

Rect SFMLRenderer::GetWindowSize() {
    Rect rect{0, 0, window.getSize().x, window.getSize().y};
    return rect;
}

void SFMLRenderer::SetShader(int shaderType) {
    if (shaderType == shaderCurrent) {
        return;
    }
    shaderCurrent = shaderType;
}

void SFMLRenderer::DrawLine(int x1, int y1, int x2, int y2, Color *c) {

    sf::Color col(c->r, c->g, c->b, c->a);

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), col),
            sf::Vertex(sf::Vector2f(x2, y2), col)
    };

    window.draw(line, 2, sf::Lines);
}

void SFMLRenderer::DrawRect(Rect *rect, Color *c, bool fill) {

    sf::Color col(c->r, c->g, c->b, c->a);
    sf::RectangleShape rectangle(sf::Vector2f(rect->w, rect->h));
    rectangle.setOutlineColor(col);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(rect->x, rect->y);
    if (fill) {
        rectangle.setFillColor(col);
    } else {
        rectangle.setFillColor(sf::Color(0, 0, 0, 0));
    }
    window.draw(rectangle);
}

void SFMLRenderer::Clip(Rect *rect) {
}

void SFMLRenderer::Clear() {
    window.clear(
            sf::Color(color.r, color.g, color.b, color.a));
}

void SFMLRenderer::Flip() {
    window.display();
}

void SFMLRenderer::Delay(unsigned int ms) {
    sf::sleep(sf::milliseconds(ms));
}

SFMLRenderer::~SFMLRenderer() {
    window.close();
}
