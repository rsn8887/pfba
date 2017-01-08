PFBA: Portable Final Burn Alpha
===========================

**The main goals of this [final burn alpha](http://www.fbalpha.com/) port are/were :**

>- have a basic but user friendly user interface on non windows devices (90%)
>- use a recent rendering interface for speed and effects support (80%)
>- cross-platform and code portability (80%)
>- easy updates of fba core (no modifications to the original sources) (100%)
>- use arm asm m68k ([cyclone](https://github.com/notaz/cyclone68000)) core on arm devices (99%)

The portability of the project is one of the main concerns, but while it's already in a good shape (few rendering functions to implement), a few class would need a better implementation like input and audio. This is something that will be worked on.

-----

**Currently supported devices**

>- Linux (sdl2)
>- RPI 3 (sdl2)
>- Ps Vita (vita2d)

The linux and rpi3 ports currently run in fullscreen mode, doesn't support filtering (shaders, wip), and only one joypad. This is something easily fixable and will be done sometime in the future.

-----

**Linux compilation**

> mkdir cmake-build
> cd cmake-build
> cmake -DCMAKE_BUILD_TYPE=Release ..
> make pfba.deps
> make pfba

**Ps Vita compilation**

> mkdir cmake-build
> cd cmake-build
> cmake -DBUILD_PSP2=ON -DCMAKE_BUILD_TYPE=Release ..
> make pfba.deps
> make pfba.vpk

**RPI3 compilation**

> mkdir cmake-build
> cd cmake-build
> cmake -DBUILD_RPI=ON -DCMAKE_BUILD_TYPE=Release ..
> make pfba.deps
> make pfba
