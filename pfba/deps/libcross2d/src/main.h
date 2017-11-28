//
// Created by cpasjuste on 08/12/16.
//

#ifndef CROSS2D_MAIN_H
#define CROSS2D_MAIN_H

#ifdef __PSP2__

#define RENDERER    PSP2Renderer
#define INPUT       SDL2Input
#define FONT_PATH   "app0:/default-20.pgf"
#define TEX_PATH    "app0:/title.png"

int KEYS[]{
        // needs to be in this order
        82, 81, 80, 79, 89, 90, 91, 92, 93, 94, 41, 40, 0 // KEY_QUIT
};

#elif __3DS__

#define RENDERER    CTRRenderer
#define INPUT       CTRInput
#define FONT_PATH   "default.ttf"
#define TEX_PATH    "title.png"

#elif __NX__

#define RENDERER    NXRenderer
#define INPUT       NXInput
#define FONT_PATH   "default.ttf"
#define TEX_PATH    "title.png"

#elif __SFML__

#define RENDERER    SFMLRenderer
#define INPUT       SFMLInput
#define FONT_PATH   "default.ttf"
#define TEX_PATH    "title.png"

int KEYS[]{
// SFML_Scancode
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

#elif __SDL2__

#define RENDERER    SDL2Renderer
#define INPUT       SDL2Input
#define FONT_PATH   "default.ttf"
#define TEX_PATH    "title.png"

int KEYS[]{
// SDL_Scancode
// needs to be in this order
        82, 81, 80, 79, 89, 90, 91, 92, 93, 94, 41, 40, 0 // KEY_QUIT
};
#endif

#endif //CROSS2D_MAIN_H
