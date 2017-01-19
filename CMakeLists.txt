cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_SYSTEM_NAME "Generic")
enable_language(ASM)

# delete cmake cache folder before changing this options
option(BUILD_PSP2 "Build with PSP2 support" ON)
option(BUILD_RPI "Build with RPI support" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Debug build")
endif ()

# setup toolchains
if (BUILD_PSP2)
    set(VITASDK "/usr/local/vitasdk")
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_COMPILER "${VITASDK}/bin/arm-vita-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${VITASDK}/bin/arm-vita-eabi-g++")
    set(CMAKE_ASM_COMPILER "${VITASDK}/bin/arm-vita-eabi-g++")
    set(CMAKE_C_FLAGS "-Wl,-q" CACHE STRING "C flags")
    execute_process(COMMAND "date" "+%Y.%m.%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(TITLE_ID "VITAFBA01")
elseif (BUILD_RPI)
    set(RPISDK "/usr/local/rpi-tools")
    set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
    set(CMAKE_C_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_ASM_COMPILER "${RPISDK}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_C_FLAGS "-I${RPISDK}/sysroot/include" CACHE STRING "C flags")
endif (BUILD_PSP2)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -fpermissive" CACHE STRING "C++ flags")

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/midway/d_kinst.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/midway/midwunit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/midway/d_wunit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/burn/drv/midway/dcs2k.cpp
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
        #src/cpu/adsp2100
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
        src/cpu/sh2
        src/cpu/tlcs90
        #src/cpu/tms34010
        src/cpu/z80
        )
file(GLOB SRC_CPU
        src/cpu/*.c*
        #src/cpu/a68k/*.c*
        #src/cpu/adsp2100/*.c*
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
        src/cpu/sh2/*.c*
        src/cpu/tlcs90/*.c*
        #src/cpu/tms34010/*.c*
        src/cpu/z80/*.c*
        )
list(REMOVE_ITEM SRC_CPU
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/adsp2100_intf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/tms34010_intf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kmake.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68k_in.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kfpu.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68k/m68kdasm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/arm7/arm7core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/arm7/arm7exec.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/hd6309/6309tbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/h6280/tblh6280.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/hd6309/6309ops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/i8051/i8051ops.c
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
        pfba/skeleton/*.c*
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
        -DLSB_FIRST -DUSE_SPEEDHACKS
        -DBUILD_SDL -DBUILD_M68K
        -DTOAPLAN_SOUND_SAMPLES_HACK
        -Wno-write-strings -Wno-narrowing
        -D_fastcall= -D__fastcall= -D__fastcall__=
        "-DINLINE=static inline" "-DSH2_INLINE=static inline"
        )
if (BUILD_DEBUG)
    list(APPEND FLAGS -O0 -g -DDEBUG)
else ()
    list(APPEND FLAGS -O3 -DNDEBUG)
endif(BUILD_DEBUG)

#################
# PSP2 (ps vita)
#################
if (BUILD_PSP2)
    list(REMOVE_ITEM SRC_CPU ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu/m68000_intf.cpp)
    list(APPEND INC
            pfba/psp2
            pfba/psp2/libvita2d/include
            pfba/psp2/vita-shader-collection/includes
            pfba/cpu/cyclone
            ${VITASDK}/arm-vita-eabi/include/freetype2
            )
    file(GLOB SRC_PSP2
            pfba/psp2/*.c*
            pfba/psp2/libvita2d/source/*.c
            pfba/sdl2/sdl2_audio.cpp # TODO: get rid of sdl2, use sdl2 audio for now
            pfba/sdl2/sdl2_input.cpp # TODO: get rid of sdl2, use sdl2 input for now
            pfba/cpu/c68000_intf.cpp
            pfba/cpu/cyclone/Cyclone.s
            )
    list(APPEND SRC_PFBA ${SRC_PSP2})
    list(APPEND FLAGS
            -Wl,-q -D__PSP2__
            -DBUILD_C68K -DUSE_FILE32API
            )
    if (BUILD_DEBUG)
        list(APPEND FLAGS -D__PSP2_DEBUG__
                -marm -mfpu=neon -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -ffast-math
                -fno-asynchronous-unwind-tables -funroll-loops
                -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
                -mvectorize-with-neon-quad -funsafe-math-optimizations
                -mlittle-endian -munaligned-access
                -O3)
    else ()
        list(APPEND FLAGS
                -marm -mfpu=neon -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -ffast-math
                -fno-asynchronous-unwind-tables -funroll-loops
                -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
                -mvectorize-with-neon-quad -funsafe-math-optimizations
                -mlittle-endian -munaligned-access
                )
    endif (BUILD_DEBUG)
    set(LDFLAGS
            SDL2
            ${CMAKE_SOURCE_DIR}/pfba/psp2/vita-shader-collection/lib/libvitashaders.a
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/clear_f_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/clear_v_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/color_f_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/color_v_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/texture_f_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/texture_v_gxp.o
            ${CMAKE_SOURCE_DIR}/pfba/psp2/libvita2d/shader/compiled/texture_tint_f_gxp.o
            SceDisplay_stub SceGxm_stub SceSysmodule_stub
            SceCtrl_stub ScePgf_stub ScePower_stub SceCommonDialog_stub
            SceAudio_stub
            freetype png jpeg z m c)
    if (BUILD_DEBUG)
        list(APPEND LDFLAGS
                psp2shell SceNetCtl_stub SceNet_stub SceAppMgr_stub)
    endif (BUILD_DEBUG)
else ()
    ##############
    # SDL2
    ##############
    list(APPEND INC
            pfba/sdl2
            )
    file(GLOB SRC_SDL2
            pfba/sdl2/*.c*
            )
    list(APPEND SRC_PFBA ${SRC_SDL2})
    list(APPEND FLAGS )
    set(LDFLAGS SDL2 SDL2_image SDL2_ttf png z)
    ##############
    # RPI
    ##############
    if (BUILD_RPI)
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
                -D__RPI__ -DBUILD_C68K -DUSE_FILE32API
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
                ${RPISDK}/sysroot/lib/libSDL2.a
                ${RPISDK}/sysroot/lib/libSDL2_image.a
                ${RPISDK}/sysroot/lib/libSDL2_ttf.a
                ${RPISDK}/sysroot/lib/libfreetype.a
                ${RPISDK}/sysroot/lib/libpng.a
                ${RPISDK}/sysroot/lib/libz.a
                pthread bcm_host rt dl)
    endif (BUILD_RPI)
endif (BUILD_PSP2)

# source files
set(SRC ${SRC_PFBA} ${SRC_CPU} ${SRC_DRV} ${SRC_BURN} ${SRC_BURNER} ${SRC_INTF} ${SRC_7Z} ${SRC_LIBCONFIG})

#####################
# SDL2 (unix) target
#####################
add_executable(${PROJECT_NAME} ${SRC} ${CMAKE_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC})
target_link_libraries(${PROJECT_NAME} ${LDFLAGS})

#####################
# PSP2 (vita) vpk
#####################
add_custom_target(${PROJECT_NAME}.vpk
        DEPENDS ${PROJECT_NAME}
        COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
        COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME} ${PROJECT_NAME}.velf
        COMMAND ${VITASDK}/bin/vita-make-fself -c ${PROJECT_NAME}.velf vpk/eboot.bin
        COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLE_ID} "${PROJECT_NAME}" vpk/sce_sys/param.sfo
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/gui/skin vpk/
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/psp2/data/* vpk
        COMMAND cd vpk && zip -r ../${PROJECT_NAME}-${BUILD_DATE}.vpk . && cd ..
        )

#########
# Deps
#########
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