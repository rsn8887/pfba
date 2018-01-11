cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")

# delete cmake cache folder before changing this option
set(BUILD_SFML OFF CACHE BOOL "Build with SFML support")
set(BUILD_SDL2 OFF CACHE BOOL "Build with SDL2 support")
set(BUILD_PSP2 OFF CACHE BOOL "Build with PSP2 support")
set(BUILD_PS3 OFF CACHE BOOL "Build with PS3 support")
set(BUILD_3DS OFF CACHE BOOL "Build with 3DS support")
set(BUILD_RPI OFF CACHE BOOL "Build with RPI support")
set(BUILD_NX OFF CACHE BOOL "Build with NX (SWITCH) support")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()

# setup toolchain
include(pfba/deps/libcross2d/Toolchain.cmake)
enable_language(ASM)

execute_process(COMMAND "date" "+%Y.%m.%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)

# add cross2d library. If you want to port pfba to another platform,
# you should (may) only need to port this library.
add_subdirectory(pfba/deps/libcross2d)

project(pfba)

# handle m68k
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/deps)
ADD_CUSTOM_COMMAND(OUTPUT
        ${CMAKE_BINARY_DIR}/deps/m68kops.h ${CMAKE_BINARY_DIR}/deps/m68kops.c
        COMMAND gcc ${CMAKE_SOURCE_DIR}/src/cpu/m68k/m68kmake.c -o ${CMAKE_BINARY_DIR}/deps/m68kmake
        COMMAND ${CMAKE_BINARY_DIR}/deps/m68kmake ${CMAKE_BINARY_DIR}/deps/ ${CMAKE_SOURCE_DIR}/src/cpu/m68k/m68k_in.c)

##############
# burn
##############
set(BURN_DIRS
        src/burn
        src/burn/devices
        src/burn/snd
        )
file(GLOB SRC_BURN
        src/burn/*.c*
        src/burn/devices/*.c*
        src/burn/snd/*.c*
        )

##############
# burner
##############
set(BURNER_DIRS
        src/burner
        )
file(GLOB SRC_BURNER
        src/burner/*.c*
        )
list(REMOVE_ITEM SRC_BURNER
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/conc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/cong.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/dat.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/gamc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/gami.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/misc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/sshot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/state.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burner/statec.cpp
        )
##############
# drivers
##############
set(DRV_DIRS
        src/burn/drv
        src/burn/drv/capcom
        src/burn/drv/cave
        src/burn/drv/coleco
        src/burn/drv/cps3
        src/burn/drv/dataeast
        src/burn/drv/galaxian
        src/burn/drv/irem
        src/burn/drv/konami
        src/burn/drv/megadrive
        src/burn/drv/midway
        src/burn/drv/msx
        src/burn/drv/neogeo
        src/burn/drv/pce
        src/burn/drv/pgm
        src/burn/drv/pre90s
        src/burn/drv/psikyo
        src/burn/drv/pst90s
        src/burn/drv/sega
        src/burn/drv/sg1000
        src/burn/drv/sms
        src/burn/drv/snes
        src/burn/drv/taito
        src/burn/drv/toaplan
        )
file(GLOB_RECURSE SRC_DRV
        src/burn/drv/*.cpp
        )
list(REMOVE_ITEM SRC_DRV
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/capcom/ctv_make.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/pgm/pgm_sprite_create.cpp
        )

##############
# interfaces
##############
set(INTF_DIRS
        src/intf
        src/intf/input
        src/intf/cd
        src/intf/audio
        )
file(GLOB SRC_INTF
        src/intf/*.cpp
        src/intf/input/*.cpp
        src/intf/cd/*.cpp
        src/intf/audio/*.cpp
        )
list(REMOVE_ITEM SRC_INTF
        ${CMAKE_CURRENT_SOURCE_DIR}/src/intf/input/inp_interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/intf/audio/aud_dsp.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/intf/audio/aud_interface.cpp
        )

##############
# cpu
##############
set(CPU_DIRS
        src/cpu
        #src/cpu/a68k
        src/cpu/adsp2100
        src/cpu/tms34010
        src/cpu/arm
        src/cpu/arm7
        src/cpu/h6280
        src/cpu/hd6309
        src/cpu/i8039
        src/cpu/i8051
        src/cpu/konami
        src/cpu/m68k
        src/cpu/m6502
        src/cpu/m6800
        src/cpu/m6805
        src/cpu/m6809
        src/cpu/mips3
        src/cpu/nec
        src/cpu/pic16c5x
        src/cpu/s2650
        src/cpu/tlcs90
        src/cpu/sh2
        src/cpu/tms32010
        src/cpu/upd7725
        src/cpu/upd7810
        src/cpu/v60
        src/cpu/z80
        )
file(GLOB SRC_CPU
        src/cpu/*.c*
        #src/cpu/a68k/*.c*
        src/cpu/adsp2100/*.c*
        src/cpu/tms34010/*.c*
        src/cpu/arm/*.c*
        src/cpu/arm7/*.c*
        src/cpu/h6280/*.c*
        src/cpu/hd6309/*.c*
        src/cpu/i8039/*.c*
        src/cpu/i8051/*.c*
        src/cpu/konami/*.c*
        src/cpu/m68k/*.c*
        src/cpu/m6502/*.c*
        src/cpu/m6800/*.c*
        src/cpu/m6805/*.c*
        src/cpu/m6809/*.c*
        src/cpu/mips3/*.c*
        src/cpu/nec/*.c*
        src/cpu/pic16c5x/*.c*
        src/cpu/s2650/*.c*
        src/cpu/tlcs90/*.c*
        src/cpu/sh2/*.c*
        src/cpu/tms32010/*.c*
        src/cpu/upd7725/*.c*
        src/cpu/upd7810/*.c*
        src/cpu/v60/*.c*
        src/cpu/z80/*.c*
        )
list(REMOVE_ITEM SRC_CPU
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/adsp2100/2100ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/tms34010/tms34010_dasm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/tms34010/tms34010_newdasm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kmake.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68k_in.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kfpu.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kdasm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/arm7/arm7core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/arm7/arm7exec.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/hd6309/6309tbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/h6280/tblh6280.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/hd6309/6309ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/konami/konamops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6502/t6502.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6502/t65c02.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6502/t65sc02.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6502/tdeco16.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6502/tn2a03.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/konami/konamtbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6800/6800ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6800/6800tbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6805/6805ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6809/6809ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m6809/6809tbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/nec/necinstr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/nec/v25sfr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/nec/v25instr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/i8051/mcs51ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/upd7810/7810ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/upd7810/7810tbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op12.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/am.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/am1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/am2.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op7a.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/am3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op2.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op4.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op6.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/op5.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/optable.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/v60mem.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/v60/v60d.c
        )

##############
# 7z
##############
set(7Z_DIRS
        src/dep/libs/lib7z
        )
file(GLOB SRC_7Z
        src/dep/libs/lib7z/7zDec.c
        src/dep/libs/lib7z/7zArcIn.c
        src/dep/libs/lib7z/7zStream.c
        src/dep/libs/lib7z/LzmaDec.c
        src/dep/libs/lib7z/Lzma2Dec.c
        src/dep/libs/lib7z/Bra.c
        src/dep/libs/lib7z/Bcj2.c
        src/dep/libs/lib7z/7zBuf.c
        src/dep/libs/lib7z/Delta.c
        src/dep/libs/lib7z/Bra86.c
        src/dep/libs/lib7z/BraIA64.c
        src/dep/libs/lib7z/7zCrc.c
        src/dep/libs/lib7z/7zCrcOpt.c
        src/dep/libs/lib7z/CpuArch.c
        )

##############
# pfba
##############
set(PFBA_DIRS
        pfba
        pfba/gui
        )
file(GLOB SRC_PFBA
        pfba/*.c*
        pfba/gui/*.c*
        )

##############
# libconfig
##############
set(LIBCONFIG_DIRS
        pfba/deps/libconfig-1.5/lib
        )
file(GLOB SRC_LIBCONFIG
        pfba/deps/libconfig-1.5/lib/grammar.c
        pfba/deps/libconfig-1.5/lib/libconfig.c
        pfba/deps/libconfig-1.5/lib/scanctx.c
        pfba/deps/libconfig-1.5/lib/scanner.c
        pfba/deps/libconfig-1.5/lib/strbuf.c
        )

# includes
set(INC ${PFBA_DIRS}
        ${7Z_DIRS}
        ${LIBCONFIG_DIRS}
        ${BURN_DIRS}
        ${BURNER_DIRS}
        ${DRV_DIRS}
        ${CPU_DIRS}
        ${INTF_DIRS}
        ${CMAKE_BINARY_DIR}/deps
        ${PROJECT_BINARY_DIR}/deps)

# flags
set(FLAGS
        #-DUNICODE
        -DINCLUDE_7Z_SUPPORT
        -DUSE_SPEEDHACKS
        -DBUILD_SDL -DBUILD_M68K
        -DTOAPLAN_SOUND_SAMPLES_HACK
        -Wno-write-strings -Wno-narrowing
        -fforce-addr -finline-limit=1200
        -DFASTCALL -U__fastcall -D_fastcall= -D__fastcall= -D__fastcall__=
        "-DINLINE=static inline" "-DSH2_INLINE=static inline"
        #-fcheck-new
        )

#################
# PSP2 (ps vita)
#################
if (BUILD_PSP2)
    list(REMOVE_ITEM SRC_CPU ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68000_intf.cpp)
    list(APPEND INC
            pfba/cpu/cyclone
            )
    file(GLOB SRC_PSP2
            pfba/cpu/c68000_intf.cpp
            pfba/cpu/cyclone/Cyclone.s
            )
    list(APPEND SRC_PFBA ${SRC_PSP2})
    list(APPEND FLAGS
            -Wl,-q -D__PSP2__ -D__PFBA_ARM__
            -DNO_KEYBOARD -DBUILD_C68K -DUSE_FILE32API -DLSB_FIRST)
    if (BUILD_DEBUG)
        list(APPEND FLAGS
                -D__PSP2_DEBUG__ # TODO: cyclone arm asm core not working without O2/O3
                )
    else ()
        list(APPEND FLAGS
                -mfpu=neon -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -ffast-math
                -fno-asynchronous-unwind-tables -funroll-loops
                -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
                -mvectorize-with-neon-quad -funsafe-math-optimizations
                -mlittle-endian -munaligned-access
                -fsingle-precision-constant
                )
    endif (BUILD_DEBUG)
    set(LDFLAGS
            cross2d SDL2
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub ScePgf_stub ScePower_stub SceCommonDialog_stub
            SceAudio_stub SceTouch_stub SceHid_stub
            freetype png jpeg z m c)
elseif (BUILD_PS3)
    ##############
    # PS3
    ##############
    list(APPEND FLAGS
            -D__PS3__
            -DNO_KEYBOARD -D__CELLOS_LV2__ -DUSE_FILE32API
            -mhard-float -ffunction-sections -fdata-sections)
    set(LDFLAGS
            cross2d
            ${PSL1GHT}/ppu/lib/libpngdec.a
            ${PSL1GHT}/portlibs/ppu/lib/libfreetype.a
            ${PSL1GHT}/portlibs/ppu/lib/libpng.a
            ${PSL1GHT}/portlibs/ppu/lib/libtiff.a
            ${PSL1GHT}/portlibs/ppu/lib/libjpeg.a
            ${PSL1GHT}/ppu/lib/libgcm_sys.a
            ${PSL1GHT}/ppu/lib/librsx.a
            ${PSL1GHT}/ppu/lib/libsysutil.a
            ${PSL1GHT}/ppu/lib/libsysmodule.a
            ${PSL1GHT}/ppu/lib/libio.a
            ${PSL1GHT}/ppu/lib/libaudio.a
            ${PSL1GHT}/portlibs/ppu/lib/libz.a)
elseif (BUILD_3DS)
    ##############
    # 3DS
    ##############
    list(APPEND FLAGS -DARM11 -D_3DS -D__3DS__
            -DNO_KEYBOARD -D__PFBA_ARM__ -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
    set(LDFLAGS
            cross2d
            ${DEVKITPRO}/portlibs/armv6k/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libz.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
elseif (BUILD_NX)
    ##############
    # NX (SWITCH)
    ##############
    list(APPEND FLAGS -D_NX -D__NX__
            -DNO_KEYBOARD -D__PFBA_ARM__ -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
    set(LDFLAGS
            cross2d
            ${DEVKITPRO}/portlibs/armv8-a/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv8-a/lib/libz.a
            ${DEVKITPRO}/libnx/lib/libnx.a
            m)
elseif (BUILD_SDL2)
    ##############
    # SDL2
    ##############
    list(APPEND FLAGS -D__SDL2__ -DLSB_FIRST)
    set(LDFLAGS
            cross2d
            SDL2
            SDL2_image
            SDL2_ttf
            png
            z)
elseif (BUILD_SFML)
    ##############
    # SFML
    ##############
    list(APPEND FLAGS -D__SFML__ -DLSB_FIRST)
    set(LDFLAGS
            cross2d
            SDL2 sfml-graphics
            sfml-window
            sfml-system
            GL
            png
            z)
    ##############
    # RPI
    ##############
elseif (BUILD_RPI)
    # add cyclone arm asm cpu support
    list(REMOVE_ITEM SRC_CPU ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68000_intf.cpp)
    list(APPEND INC
            pfba/cpu/cyclone
            )
    file(GLOB SRC_RPI
            pfba/cpu/c68000_intf.cpp
            pfba/cpu/cyclone/Cyclone.s
            )
    list(APPEND SRC_PFBA ${SRC_RPI})
    # rpi3 compile flags
    list(APPEND FLAGS
            -D__RPI__ -D__PFBA_ARM__ -DBUILD_C68K -DUSE_FILE32API -DLSB_FIRST
            -march=armv8-a -mcpu=cortex-a53 -mtune=cortex-a53 -mfpu=neon-fp-armv8
            -mfloat-abi=hard -mvectorize-with-neon-quad -funsafe-math-optimizations
            -ffast-math -mlittle-endian -munaligned-access
            -fno-asynchronous-unwind-tables -funroll-loops
            -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
            )
    # rpi linker flags, linked to raspbian binaries compiled on
    # the rpi device with no x11 support for sdl2 (faster, more stable)
    set(LDFLAGS
            -s
            -Xlinker -rpath-link=${RPISDK}/sysroot/lib
            -L${RPISDK}/sysroot/lib
            cross2d
            ${RPISDK}/sysroot/lib/libSDL2.a
            ${RPISDK}/sysroot/lib/libSDL2_image.a
            ${RPISDK}/sysroot/lib/libSDL2_ttf.a
            ${RPISDK}/sysroot/lib/libfreetype.a
            ${RPISDK}/sysroot/lib/libpng.a
            ${RPISDK}/sysroot/lib/libz.a
            pthread bcm_host rt dl)
endif (BUILD_PSP2)

# source files
set(SRC ${SRC_PFBA} ${SRC_CPU} ${SRC_DRV} ${SRC_BURN} ${SRC_BURNER} ${SRC_INTF} ${SRC_7Z} ${SRC_LIBCONFIG})

############
# FBA deps
############
add_custom_target(${PROJECT_NAME}.deps ALL
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/deps && mkdir ${PROJECT_BINARY_DIR}/deps
        # driverlist.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/gamelist.pl -o deps/driverlist.h -l deps/gamelist.txt ${SRC_DRV}
        # ctv.h
        COMMAND g++ ${CMAKE_SOURCE_DIR}/src/burn/drv/capcom/ctv_make.cpp -o deps/ctv_make
        COMMAND ${PROJECT_BINARY_DIR}/deps/ctv_make > deps/ctv.h
        # pgm sprite
        COMMAND g++ ${CMAKE_SOURCE_DIR}/src/burn/drv/pgm/pgm_sprite_create.cpp -o deps/pgm_sprite_create
        COMMAND ${PROJECT_BINARY_DIR}/deps/pgm_sprite_create > deps/pgm_sprite.h
        # func headers
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/toa_gp9001_func.pl -o deps/toa_gp9001_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/neo_sprite_func.pl -o deps/neo_sprite_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/cave_tile_func.pl -o deps/cave_tile_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/cave_sprite_func.pl -o deps/cave_sprite_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/src/dep/scripts/psikyo_tile_func.pl -o deps/psikyo_tile_func.h
        )

##########################
# main target
##########################
add_executable(${PROJECT_NAME}.elf ${SRC} ${CMAKE_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INC})
target_link_libraries(${PROJECT_NAME}.elf ${LDFLAGS})

#####################
# PSP2 (vita) vpk
#####################
add_custom_target(${PROJECT_NAME}.vpk
        DEPENDS ${PROJECT_NAME}.elf
        COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
        COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
        COMMAND ${VITASDK}/bin/vita-make-fself -s -c ${PROJECT_NAME}.velf vpk/eboot.bin
        COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLE_ID} "${PROJECT_NAME}" vpk/sce_sys/param.sfo
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/data/skin vpk/
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/data/psp2/* vpk
        COMMAND cd vpk && zip -r ../${PROJECT_NAME}-${BUILD_DATE}.vpk . && cd ..
        )

#####################
# PS3
#####################
add_custom_target(${PROJECT_NAME}.ps3
        DEPENDS ${PROJECT_NAME}.elf
        COMMAND ${PSL1GHT}/bin/sprxlinker ${PROJECT_NAME}.elf
        COMMAND ${PSL1GHT}/bin/make_self ${PROJECT_NAME}.elf ${PROJECT_NAME}.ps3
        )

#####################
# CTR (3DS) target
#####################
if (BUILD_3DS)
    add_executable(${PROJECT_NAME}.3ds ${SRC} ${CMAKE_BINARY_DIR}/deps/m68kops.c)
    target_compile_options(${PROJECT_NAME}.3ds PRIVATE ${FLAGS})
    target_include_directories(${PROJECT_NAME}.3ds PRIVATE
            ${INC}
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/armv6k/include
            )
    target_link_libraries(${PROJECT_NAME}.3ds
            cross2d
            ${DEVKITPRO}/portlibs/3ds/lib/libSDL2.a
            ${LDFLAGS})
    set_target_properties(${PROJECT_NAME}.3ds PROPERTIES LINK_FLAGS "-specs=3dsx.specs")
    add_3dsx_target(${PROJECT_NAME}.3ds "pFBA" "FBA 3DS" "Cpasjuste" pfba/data/3ds/icon.png)
    add_cia_target(${PROJECT_NAME}.3ds pfba/data/3ds/app.rsf pfba/data/3ds/banner.png pfba/data/3ds/audio.wav
            "pFBA" "FBA 3DS" "Cpasjuste" pfba/data/3ds/icon.png
            )
endif (BUILD_3DS)

#####################
# NX (SWITCH) target
#####################
add_executable(${PROJECT_NAME}.nx ${SRC} ${CMAKE_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME}.nx PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME}.nx PRIVATE
        ${INC}
        ${DEVKITPRO}/libnx/include
        ${DEVKITPRO}/portlibs/armv8-a/include
        )
target_link_libraries(${PROJECT_NAME}.nx cross2d ${LDFLAGS})
set_target_properties(${PROJECT_NAME}.nx PROPERTIES LINK_FLAGS "-specs=switch.specs")
add_custom_target(${PROJECT_NAME}.nro
        DEPENDS ${PROJECT_NAME}.nx
        COMMAND ${DEVKITPRO}/devkitA64/bin/elf2nro ${PROJECT_NAME}.nx ${PROJECT_NAME}.nro
        )
