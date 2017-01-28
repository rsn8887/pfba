//
// Created by cpasjuste on 08/12/16.
//

#include <skeleton/renderer.h>
#include <skeleton/input.h>

#ifdef __PSP2_DEBUG__
#include <psp2shell.h>
#endif

#define SCRW 960
#define SCRH 544

Renderer *renderer;
Font *font;
Input *input;

int main() {

#ifdef __PSP2__
#ifdef __PSP2_DEBUG__
    psp2shell_init(3333, 0);
#endif
    renderer = (Renderer *) new PSP2Renderer(SCRW, SCRH);
    input = (Input *) new SDL2Input();
    font = renderer->LoadFont("app0:/skin/default.pgf", 21);
#elif __SFML__
    renderer = (Renderer *) new SFMLRenderer(SCRW, SCRH, "");
    input = (Input *) new SFMLInput((SFMLRenderer*)renderer);
    font = renderer->LoadFont("/home/cpasjuste/dev/psvita/libcross2d/src/res/default.ttf", 60);
#else
    renderer = (Renderer *) new SDL2Renderer(SCRW, SCRH);
    input = (Input *) new SDL2Input();
    font = renderer->LoadFont("/home/cpasjuste/dev/psvita/libcross2d/src/res/default.ttf", 60);
#endif

    int sdl_keyboard_keys[] {
            // SDL_Scancode
            // needs to be in this order
            82, 81, 80, 79, 89, 90,
            91, 92, 93, 94, 41, 40,
            0 // KEY_QUIT
    };
    int sfml_keyboard_keys[] {
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

        renderer->Clear();

        // window
        rect = {0, 0, renderer->GetWindowSize().w, renderer->GetWindowSize().h};
        renderer->DrawRect(rect, ORANGE, false);

        // inside rect
        rect.x+=1; rect.y+=1; rect.w-=2; rect.h-=2;
        renderer->DrawRect(rect, GRAY_LIGHT);

        // inside "inside rect"
        rect.x+=1; rect.y+=1; rect.w-=2; rect.h-=2;
        renderer->DrawRect(rect, GREEN, false);

        // "inside rect" inside border
        renderer->DrawBorder(rect, ORANGE);

        // screen centers
        renderer->DrawLine(0, rect.h/2, rect.w, rect.h/2); // X
        renderer->DrawLine(rect.w/2, 0, rect.w/2, rect.h); // Y

        // ...
        renderer->DrawFont(font, 0, 0, "HELLO WORLD");

        // ...
        Rect r{ rect.w/2, rect.h/2, 0, 0 };
        r.w = font->GetWidth("HELLO WORLD");
        r.h = font->GetHeight("HELLO WORLD");
        r.x -= r.w/2;
        r.y -= r.h/2;
        renderer->DrawRect(r, RED, false);
        renderer->DrawFont(font, r, WHITE, true, true, "HELLO WORLD");

        // truncate text
        rect.x = 0;
        rect.y = 0;
        rect.w = 50;
        renderer->DrawFont(font, rect, WHITE, false, true, "HELLO WORLD");

        renderer->Flip();
        //renderer->Delay(100);

        Input::Player player = input->Update()[0];
        if(player.state) {
            if(player.state & EV_QUIT)
                break;
            printf("state: %i\n", player.state);
        }
    }

    delete (font);
    delete (renderer);
    delete (input);
}
