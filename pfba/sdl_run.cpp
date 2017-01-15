/*
 * FinalBurn Alpha for Dingux/OpenDingux
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <sys/time.h>

#include "burner.h"
#include "sdl_run.h"
#include <skeleton/audio.h>
#include <sdl2/sdl2_audio.h>
#include <video.h>

static Gui *gui;
Video *video;
Audio *audio;

extern int nSekCpuCore;
struct timeval start;

bool GameLooping;
bool bPauseOn = false;

int InpMake(Input::Player *players);

unsigned int GetTicks(void) {
    unsigned int ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (unsigned int) ((now.tv_sec - start.tv_sec) * 1000000 + now.tv_usec - start.tv_usec);
    return ticks;
}

int RunReset() {
    nFramesEmulated = 0;
    nCurrentFrame = 0;
    nFramesRendered = 0;
    return 0;
}


int RunOneFrame(bool bDraw, int bDrawFps, int fps) {

    int rotation = gui->GetConfig()->GetRomValue(Option::Index::ROM_ROTATION);
    bool rotate = (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) && !rotation;

    Input::Player *players = gui->GetInput()->Update(rotate);
    InpMake(players);

    // process menu
    if ((players[0].state & Input::Key::KEY_COIN)
        && (players[0].state & Input::Key::KEY_START)) {
        bPauseOn = true;
        audio->Pause(1);
        gui->RunOptionMenu(true);
        audio->Pause(0);
        bPauseOn = false;
    } else if ((players[0].state & Input::Key::KEY_COIN)
               && (players[0].state & Input::Key::KEY_FIRE5)) {
        bPauseOn = true;
        audio->Pause(1);
        gui->RunStatesMenu();
        audio->Pause(0);
        bPauseOn = false;
    }

    if (!bPauseOn) {
        nFramesEmulated++;
        nCurrentFrame++;

        pBurnDraw = NULL;
        if (bDraw) {
            nFramesRendered++;
            video->Lock();
        }
        BurnDrvFrame();

        if (bDraw) {
            if (bDrawFps) {
                video->Clear();
            }
            video->Unlock();
            video->Render();
            if (bDrawFps) {
                gui->GetSkin()->font->color = YELLOW;
                video->renderer->DrawFont(gui->GetSkin()->font, 8, 8, "FPS: %2d/%2d", fps, (nBurnFPS / 100));
                gui->GetSkin()->font->color = WHITE;
            }
            video->Flip();
        }
    }

    return 0;
}

void RunEmulator(Gui *g, int drvnum) {

    printf("RunEmulator '%s'\n", BurnDrvGetTextA(DRV_FULLNAME));

    // set gui pointer
    gui = g;

#if defined(__PSP2__) || defined(__RPI__)
    nSekCpuCore = 0; // SEK_CORE_C68K: USE CYCLONE ARM ASM M68K CORE
    int hardware = BurnDrvGetHardwareCode();
    if (hardware & HARDWARE_SEGA_FD1089A_ENC
        || hardware & HARDWARE_SEGA_FD1089B_ENC
        || hardware & HARDWARE_SEGA_MC8123_ENC
        || hardware & HARDWARE_SEGA_FD1094_ENC
        || hardware & HARDWARE_SEGA_FD1094_ENC_CPU2) {
        nSekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
        gui->MessageBox("ROM IS CRYPTED, USE DECRYPTED ROM (CLONE)\n"
                                "TO ENABLE CYCLONE ASM CORE (FASTER)", "OK", NULL);
    }
#endif
    bForce60Hz = true;

    InpInit();
    InpDIP();

    // audio needs to be inited before rom driver...
    printf("Creating audio device\n");

    int frequency = 44100;
    switch (gui->GetConfig()->GetRomValue(Option::Index::ROM_AUDIO_FREQ)) {
        case 0:
            frequency = 0;
            break;
        case 1:
            frequency = 11025;
            break;
        case 2:
            frequency = 22050;
            break;
        case 4:
            frequency = 48000;
            break;
        default:
            break;
    }

    audio = (Audio *) new SDL2Audio(frequency, nBurnFPS);
    if (audio->available) {
        nBurnSoundRate = audio->frequency;
        nBurnSoundLen = audio->buffer_len;
        pBurnSoundOut = audio->buffer;
    }

    printf("Initialize rom driver\n");
    if (DrvInit(drvnum, false) != 0) {
        printf("Driver initialisation failed! Likely causes are:\n"
                       "- Corrupt/Missing ROM(s)\n"
                       "- I/O Error\n"
                       "- Memory error\n\n");
        DrvExit();
        InpExit();
        delete (audio);
        return;
    }

    printf("Creating video device\n");
    video = new Video(gui->GetRenderer());

    // set per rom input scheme
    gui->SetPlayerInputMapping(true);

    RunReset();

    int now, done = 0, timer = 0, ticks = 0, tick = 0, i = 0, fps = 0;
    unsigned int frame_limit = (unsigned int) (nBurnFPS / 100),
            frame_time = (unsigned int) (100000000 / nBurnFPS);

    gettimeofday(&start, NULL);

    GameLooping = true;
    printf("---- PFBA EMU START ----\n\n");

    while (GameLooping) {

        // audio lag when waiting for audio, force "frameskip" when audio enabled
        int frameSkip = frequency;
        int showFps = gui->GetConfig()->GetRomValue(Option::Index::ROM_SHOW_FPS);

        if (frameSkip) {
            timer = GetTicks() / frame_time;;
            if (timer - tick > frame_limit && showFps) {
                fps = nFramesRendered;
                nFramesRendered = 0;
                tick = timer;
            }
            now = timer;
            ticks = now - done;
            if (ticks < 1) continue;
            if (ticks > 10) ticks = 10;
            for (i = 0; i < ticks - 1; i++) {
                RunOneFrame(false, showFps, fps);
                audio->Play();
            }
            if (ticks >= 1) {
                RunOneFrame(true, showFps, fps);
                audio->Play();
            }
            done = now;
        } else {
            if (showFps) {
                timer = GetTicks();
                if (timer - tick > 1000000) {
                    fps = nFramesRendered;
                    nFramesRendered = 0;
                    tick = timer;
                }
            }
            RunOneFrame(true, showFps, fps);
            audio->Play();
        }
    }

    printf("---- PFBA EMU END ----\n\n");

    DrvExit();
    InpExit();
    delete (video);
    video = NULL;
    delete (audio);
    audio = NULL;

    gui->SetPlayerInputMapping(false);
}
