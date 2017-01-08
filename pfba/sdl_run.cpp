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
#include "sdl_input.h"
#include <video.h>
#include <skeleton/audio.h>
#include <sdl2/sdl2_audio.h>

static Gui *gui;
Video *video;
Audio *audio;

extern int nSekCpuCore;
struct timeval start;
extern unsigned int FBA_KEYPAD[4];

bool GameLooping;
bool bPauseOn = false;

int InpMake(unsigned int[]);

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

    sdl2_input_read();
    InpMake(FBA_KEYPAD);

    if (!bPauseOn) {
        nFramesEmulated++;
        nCurrentFrame++;

        pBurnDraw = NULL;
        if (bDraw) {
            nFramesRendered++;
            video->Lock();
        }
        BurnDrvFrame();
        //pBurnDraw = NULL;

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

#ifdef __PSP2__
    printf("nSekCpuCore '%i'\n", gui->GetConfig()->GetRomValue(Option::Index::ROM_M68K));
    nSekCpuCore = gui->GetConfig()->GetRomValue(Option::Index::ROM_M68K);
#endif
    bForce60Hz = true;

    InpInit();
    InpDIP();

    // audio needs to be inited before rom driver...
    printf("Creating audio device\n");
    int freq_index = gui->GetConfig()->GetRomValue(Option::Index::ROM_AUDIO_FREQ);
    audio = (Audio*)new SDL2Audio(freq_index);

    printf("Initialize rom driver\n");
    if (DrvInit(drvnum, false) != 0) {
        printf("Driver initialisation failed! Likely causes are:\n"
                       "- Corrupt/Missing ROM(s)\n"
                       "- I/O Error\n"
                       "- Memory error\n\n");
        DrvExit();
        InpExit();
        delete(audio);
        return;
    }

    printf("Creating video device\n");
    video = new Video(gui->GetRenderer());

    RunReset();

    int now, done = 0, timer = 0, ticks = 0, tick = 0, i = 0, fps = 0;
    unsigned int frame_limit = (unsigned int) (nBurnFPS / 100),
            frame_time = (unsigned int) (100000000 / nBurnFPS);

    gettimeofday(&start, NULL);
    GameLooping = true;

    printf("Let's go!\n");

    while (GameLooping) {

        // audio lag when not using frameskip, force frameskip when audio enabled
        //int frameSkip = gui->GetConfig()->GetRomValue(Option::Index::ROM_FRAMESKIP);
        int frameSkip = freq_index;
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

    printf("---- Shutdown Finalburn Alpha plus ----\n\n");
    DrvExit();
    InpExit();
    delete (video);
    video = NULL;
    delete(audio);
    audio = NULL;
}
