PFBA: Portable Final Burn Alpha
===========================

![](http://files.mydedibox.fr/files/Dev/psp2/pfba/screen1-mini.jpg)


**The main goals of this [final burn alpha](http://www.fbalpha.com/) port are/were :**

>- have a basic but user friendly user interface on non windows devices
>- use a recent rendering interface for speed and effects support
>- cross-platform and code portability
>- easy updates of fba core (no modifications to the original sources)
>- use arm asm m68k ([cyclone](https://github.com/notaz/cyclone68000)) core on arm devices

The portability of the project is one of the main concerns, but while it's already in a good shape (few rendering functions to implement), some code would still need a better implementation.

-----

**Currently supported devices**

>- Linux (sdl2, wip)
>- RPI 3 (sdl2, wip)
>- Ps Vita (vita2d)

The linux and rpi3 ports currently run in fullscreen mode, doesn't support filtering (shaders, wip), and only one joypad. This is something easily fixable and will be done sometime in the future.

-----

**PS VITA**

>- data is located at "ux0:/data/pfba"
>- default rom path is "ux0:/data/pfba/roms/"
>- rom path can be changed in "ux0:/data/pfba/pfba.cfg"
>- preview images can be put in "ux0:/data/pfba/previews/rom-name.png"
>- in the ui: START = gui options + default rom configuration
>- in the ui: SELECT = selected rom configuration
>- in game: START+SELECT = menu
>- in game: START+L = saves states

-----

**Linux compilation**

>- mkdir cmake-build
>- cd cmake-build
>- cmake -DCMAKE_BUILD_TYPE=Release ..
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
>- cmake -DBUILD_RPI=ON -DCMAKE_BUILD_TYPE=Release ..
>- make pfba.deps
>- make pfba

**Developers tips**

>-

-----

**Thanks**

>- Xavier for the support (http://www.consolex-bordeaux.fr/)
>- ScHlAuChi for all the advices and testing
>- fba/mame authors (http://www.fbalpha.com/, http://mamedev.org/)
>- xerpi for vita2d (https://github.com/xerpi/libvita2d)
>- frangarcj for vita2d shader support (https://github.com/frangarcj/vita2dlib)
>- people behind the vitasdk (https://github.com/vitasdk)
