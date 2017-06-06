//
// Created by cpasjuste on 08/12/16.
//

#include "skeleton/renderer.h"
#include "skeleton/input.h"

#define SCRW 960
#define SCRH 544

Renderer *renderer;
Font *font_small;
Font *font_large;
Input *input;

int main() {

#ifdef __PSP2__
    renderer = (Renderer *) new PSP2Renderer(SCRW, SCRH);
    input = (Input *) new SDL2Input();
    font_small = renderer->LoadFont("app0:/default-20.pgf", 20); // 20 = pgf font size
    font_large = renderer->LoadFont("app0:/default-40.pgf", 40); // 40 = pgf font size
#elif __3DS__
    renderer = (Renderer*) new CTRRenderer();
    input = (Input *) new CTRInput();
    font_small = renderer->LoadFont("default.ttf", 20);
    font_large = renderer->LoadFont("default.ttf", 40);
#elif __SFML__
    renderer = (Renderer *) new SFMLRenderer(SCRW, SCRH);
    input = (Input *) new SFMLInput((SFMLRenderer*)renderer);
#else
    renderer = (Renderer *) new SDL2Renderer(SCRW, SCRH);
    input = (Input *) new SDL2Input();
#endif

    int sdl_keyboard_keys[]{
            // SDL_Scancode
            // needs to be in this order
            82, 81, 80, 79, 89, 90,
            91, 92, 93, 94, 41, 40,
            0 // KEY_QUIT
    };
    int sfml_keyboard_keys[]{
            // SDL_Scancode
            // needs to be in this order
            73, // KEY_UP
            74, // KEY_DOWN
            71, // KEY_LEFT
            72, // KEY_RIGHT
            36, // KEY_COINS (SELECT)
            58, // KEY_START
            91,
            92,
            93,
            94,
            41,
            40,
            0 // KEY_QUIT
    };

    input->SetKeyboardMapping(sfml_keyboard_keys);

    Rect rect;

    while (true) {

        Input::Player player = input->Update()[0];
        if (player.state) {
            if (player.state & EV_QUIT)
                break;
            printf("state: %i\n", player.state);
            renderer->Delay(100);
        }

        renderer->Clear();

        // window
        rect = {0, 0, renderer->GetWindowSize().w, renderer->GetWindowSize().h};
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
        renderer->DrawFont(font_large, rect.w / 2, 0, "HELLO WORLD");

        // top left text
        renderer->DrawFont(font_small, 0, 0, "HELLO WORLD");

        // centered text
        Rect r{rect.w / 2, rect.h / 2, 0, 0};
        font_small->scaling = 0.8;
        r.w = font_small->GetWidth("HELLO WORLD");
        r.h = font_small->GetHeight("HELLO WORLD");
        r.x -= r.w / 2;
        r.y -= r.h / 2;
        renderer->DrawRect(r, RED, false);
        renderer->DrawFont(font_small, r, WHITE, true, true, "HELLO WORLD");
        font_small->scaling = 1;

        // y centered and truncated text
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        renderer->DrawFont(font_small, rect, WHITE, false, true, "HELLO WORLD");

        renderer->Flip();
    }

    delete (font_small);
    delete (font_large);
    delete (renderer);
    delete (input);
}
