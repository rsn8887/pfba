//
// Created by cpasjuste on 08/12/16.
//

#include "main.h"

#include "skeleton/renderer.h"
#include "skeleton/input.h"

// Platform specific code in main.h

#define SCRW 960
#define SCRH 544

int main() {

    Renderer *renderer = (Renderer *) new RENDERER(SCRW, SCRH);

    Input *input = nullptr;
#ifdef __SFML__
    input = (Input *) new INPUT((SFMLRenderer *) renderer);
#else
    input = (Input *) new INPUT();
#endif
    input->SetKeyboardMapping(KEYS);

    Font *font = renderer->LoadFont(FONT_PATH, 30);
    Texture *tex = renderer->LoadTexture(TEX_PATH);

    Rect rect{};

    while (true) {

        Input::Player player = input->Update()[0];
        if (player.state) {
            if (player.state & EV_QUIT)
                break;
            //printf("state: %i\n", player.state);
            renderer->Delay(100);
        }

        renderer->Clear();

        // window
        rect = {0, 0, renderer->GetWindowSize().w - 1, renderer->GetWindowSize().h - 1};
        renderer->DrawRect(rect, ORANGE, false);

        // inside rect
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
        renderer->DrawRect(rect, GRAY_LIGHT);

        // inside "inside rect"
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
        renderer->DrawRect(rect, GREEN, false);

        // "inside rect" inside border
        renderer->DrawBorder(rect, ORANGE);

        // screen centers
        renderer->DrawLine(0, rect.h / 2, rect.w, rect.h / 2); // X
        renderer->DrawLine(rect.w / 2, 0, rect.w / 2, rect.h); // Y

        // top middle text
        renderer->DrawFont(font, rect.w / 2, 0, "HELLO WORLD");

        // top left text
        renderer->DrawFont(font, 0, 0, "HELLO WORLD");

        // centered text
        Rect r{rect.w / 2, rect.h / 2, 0, 0};
        font->scaling = 2;
        r.w = font->GetWidth("HELLO WORLD");
        r.h = font->GetHeight("HELLO WORLD");
        r.x -= r.w / 2;
        r.y -= r.h / 2;
        renderer->DrawRect(r, RED, false);
        renderer->DrawFont(font, r, WHITE, true, true, "HELLO WORLD SCALED");
        font->scaling = 1;

        // y centered and truncated text
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        renderer->DrawFont(font, rect, WHITE, false, true, "HELLO WORLD");

        //renderer->DrawTexture(tex, 64, 64);

        renderer->Flip();
    }

    delete (input);
    delete (font);
    delete (renderer);
}
