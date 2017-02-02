PFBA: Portable Final Burn Alpha
===========================

![](http://files.mydedibox.fr/files/Dev/psp2/pfba/screen1-mini.jpg)


**The main goals of this [final burn alpha](http://www.fbalpha.com/) port are/were :**

>- have a basic but user friendly user interface on non windows devices
>- use a recent rendering interface for speed and effects support
>- cross-platform and code portability
>- easy updates of fba core (no modifications to the original sources)
>- use arm asm m68k ([cyclone](https://github.com/notaz/cyclone68000)) core on arm devices

-----

**Currently supported devices**

>- Linux (sfml, sdl2)
>- RPI 3 (sfml, sdl2, wip)
>- Ps Vita (vita2d)

-----

**PS VITA**

>- data is located at "ux0:/data/pfba"
>- default rom path is "ux0:/data/pfba/roms/"
>- rom path can be changed in "ux0:/data/pfba/pfba.cfg"
>- preview images can be put in "ux0:/data/pfba/previews/rom-name.png"
>- in the ui: START = gui options + default rom configuration
>- in the ui: SELECT = selected rom configuration
>- in game: SELECT+START = menu
>- in game: SELECT+L = saves states
>- in game: SELECT+SQUARE = service switch
>- in game: SELECT+TRIANGLE = 1P+2P switch
>- in game: SELECT+LEFT/RIGHT = change shader
>- in game: SELECT+UP/DOWN = change scaling

-----

**Linux compilation**

>- mkdir cmake-build
>- cd cmake-build
>- cmake -DBUILD_SFML=ON -DCMAKE_BUILD_TYPE=Release ..
>- make pfba.deps
>- make pfba

**Ps Vita compilation**

>- mkdir cmake-build
>- cd cmake-build
>- cmake -DBUILD_PSP2=ON -DCMAKE_BUILD_TYPE=Release ..
>- make pfba.deps
>- make pfba.vpk

**RPI3 compilation**

>- mkdir cmake-build
>- cd cmake-build
>- cmake -DBUILD_RPI=ON -DBUILD_SDL2=ON -DCMAKE_BUILD_TYPE=Release ..
>- make pfba.deps
>- make pfba

**Developers tips**

There is currently two modifications to the original FBA sources :
>- [ARM: fix nec cpu (irem, raiden...)](https://github.com/Cpasjuste/pfba/commit/d1d8bc51457a1f00eecc5a934b94afe5993c7eae)
>- [COMMON: fix eeprom (nvram) loading/saving](https://github.com/Cpasjuste/pfba/commit/e0c5153a530fedcf267e50e047afc8770a2d4ef4)

-----

**Thanks**

>- Xavier for the support (http://www.consolex-bordeaux.fr/)
>- ScHlAuChi for all the advices and testing
>- fba/mame authors (http://www.fbalpha.com/, http://mamedev.org/)
>- xerpi for vita2d (https://github.com/xerpi/libvita2d)
>- frangarcj for vita2d shader support (https://github.com/frangarcj/vita2dlib)
>- people behind the vitasdk (https://github.com/vitasdk)
