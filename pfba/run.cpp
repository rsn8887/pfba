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
#include "run.h"
#include <skeleton/audio.h>
#include <video.h>

static Gui *gui;
Video *video;
Audio *audio;

extern unsigned char inputServiceSwitch;
extern unsigned char inputP1P2Switch;
extern int nSekCpuCore;

bool GameLooping;
bool bPauseOn = false;

int InpMake(Input::Player *players);

struct timeval start;

void StartTicks(void) {
    gettimeofday(&start, NULL);
}

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

    inputServiceSwitch = 0;
    inputP1P2Switch = 0;

    int rotation = gui->GetConfig()->GetRomValue(Option::Index::ROM_ROTATION);
    int rotate = 0;
    if (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        if (rotation == 0) {
            //rotate controls by 90 degrees
            rotate = 1;
        }
        if (rotation == 2) {
            //rotate controls by 270 degrees
            rotate = 3;
        }
    }
    Input::Player *players = gui->GetInput()->Update(rotate);

    // process menu
    if ((players[0].state & Input::Key::KEY_MENU1)
        && (players[0].state & Input::Key::KEY_MENU2)) {
#ifdef	FAST_EXIT
        gui->GetInput()->Clear(0);
        GameLooping = false;
#else
        bPauseOn = true;
        if (audio) {
            audio->Pause(1);
        }
        // set default control scheme
        gui->UpdateInputMapping(false);
        gui->RunOptionMenu(true);
        // restore rom control scheme
        gui->UpdateInputMapping(true);
        if (audio) {
            audio->Pause(0);
        }
        bPauseOn = false;
#endif
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_FIRE5)) {
        bPauseOn = true;
        if (audio) {
            audio->Pause(1);
        }
        // set default control scheme
        gui->UpdateInputMapping(false);
        gui->RunStatesMenu();
        // restore rom control scheme
        gui->UpdateInputMapping(true);
        if (audio) {
            audio->Pause(0);
        }
        bPauseOn = false;
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_FIRE3)) {
        inputServiceSwitch = 1;
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_FIRE4)) {
        inputP1P2Switch = 1;
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_UP)) {
        int scaling = gui->GetConfig()->GetRomValue(Option::Index::ROM_SCALING) + 1;
        if (scaling <= 5) {
            int index = gui->GetConfig()->GetOptionPos(gui->GetConfig()->GetRomOptions(),
                                                       Option::Index::ROM_SCALING);
            gui->GetConfig()->GetRomOptions()->at(index).value = scaling;
            video->Scale();
            gui->GetRenderer()->Delay(500);
        }
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_DOWN)) {
        int scaling = gui->GetConfig()->GetRomValue(Option::Index::ROM_SCALING) - 1;
        if (scaling >= 0) {
            int index = gui->GetConfig()->GetOptionPos(gui->GetConfig()->GetRomOptions(),
                                                       Option::Index::ROM_SCALING);
            gui->GetConfig()->GetRomOptions()->at(index).value = scaling;
            video->Scale();
            gui->GetRenderer()->Delay(500);
        }
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_RIGHT)) {
        int shader = gui->GetConfig()->GetRomValue(Option::Index::ROM_SHADER) + 1;
        if (shader < gui->GetRenderer()->shaders->Count()) {
            int index = gui->GetConfig()->GetOptionPos(gui->GetConfig()->GetRomOptions(),
                                                       Option::Index::ROM_SHADER);
            gui->GetConfig()->GetRomOptions()->at(index).value = shader;
            gui->GetRenderer()->SetShader(shader);
            gui->GetRenderer()->Delay(500);
        }
    } else if ((players[0].state & Input::Key::KEY_MENU2)
               && (players[0].state & Input::Key::KEY_LEFT)) {
        int shader = gui->GetConfig()->GetRomValue(Option::Index::ROM_SHADER) - 1;
        if (shader >= 0) {
            int index = gui->GetConfig()->GetOptionPos(gui->GetConfig()->GetRomOptions(),
                                                       Option::Index::ROM_SHADER);
            gui->GetConfig()->GetRomOptions()->at(index).value = shader;
            gui->GetRenderer()->SetShader(shader);
            gui->GetRenderer()->Delay(500);
        }
    } else if (players[0].state & EV_RESIZE) {
        video->Scale();
    }

    InpMake(players);

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
                gui->GetSkin()->font->color = C2D_COL_YELLOW;
                gui->GetSkin()->font->Draw(8, 8, "FPS: %2d/%2d", fps, (nBurnFPS / 100));
                gui->GetSkin()->font->color = C2D_COL_WHITE;
            }
            video->Flip();
        }
    }

    return 0;
}

#if defined(__PSP2__) || defined(__RPI__)

static int GetSekCpuCore(Gui *g) {

    int sekCpuCore = 0; // SEK_CORE_C68K: USE CYCLONE ARM ASM M68K CORE
    // int sekCpuCore = g->GetConfig()->GetRomValue(Option::Index::ROM_M68K);

    std::vector<std::string> zipList;
    int hardware = BurnDrvGetHardwareCode();
    
    if (!g->GetConfig()->GetRomValue(Option::Index::ROM_NEOBIOS)
        && RomList::IsHardware(hardware, HARDWARE_PREFIX_SNK)) {
        sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
        g->MessageBox("UNIBIOS DOESNT SUPPORT THE M68K ASM CORE\n"
                             "CYCLONE ASM CORE DISABLED", "OK", NULL);
    }

    if (RomList::IsHardware(hardware, HARDWARE_PREFIX_SEGA)) {
        if (hardware & HARDWARE_SEGA_FD1089A_ENC
            || hardware & HARDWARE_SEGA_FD1089B_ENC
            || hardware & HARDWARE_SEGA_MC8123_ENC
            || hardware & HARDWARE_SEGA_FD1094_ENC
            || hardware & HARDWARE_SEGA_FD1094_ENC_CPU2) {
            sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
            g->MessageBox("ROM IS CRYPTED, USE DECRYPTED ROM (CLONE)\n"
                                  "TO ENABLE CYCLONE ASM CORE (FASTER)", "OK", NULL);
        }
    } else if (RomList::IsHardware(hardware, HARDWARE_PREFIX_TOAPLAN)) {
        zipList.push_back("batrider");
        zipList.push_back("bbakraid");
        zipList.push_back("bgaregga");
    } else if (RomList::IsHardware(hardware, HARDWARE_PREFIX_SNK)) {
        zipList.push_back("kof97");
        zipList.push_back("kof98");
        zipList.push_back("kof99");
        zipList.push_back("kof2000");
        zipList.push_back("kof2001");
        zipList.push_back("kof2002");
        zipList.push_back("kf2k3pcb");
        //zipList.push_back("kof2003"); // WORKS
    }

    std::string zip = BurnDrvGetTextA(DRV_NAME);
    for (int i = 0; i < zipList.size(); i++) {
        if (zipList[i].compare(0, zip.length(), zip) == 0) {
            g->MessageBox("THIS ROM DOESNT SUPPORT THE M68K ASM CORE\n"
                                  "CYCLONE ASM CORE DISABLED", "OK", NULL);
            sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
            break;
        }
    }

    return sekCpuCore;
}

#endif

void AudioInit(Config *cfg) {

    // disable interpolation as it produce "cracking" sound
    // on some games (cps1 (SF2), cave ...)
    nInterpolation = 1;
    //cfg->GetRomValue(Option::Index::ROM_AUDIO_INTERPOLATION) == 0 ? 1 : 3;
    nFMInterpolation = 0;
    //cfg->GetRomValue(Option::Index::ROM_AUDIO_FMINTERPOLATION) == 0 ? 0 : 3;

#ifdef __3DS__
    //nBurnSoundRate = 0;
    //nBurnSoundLen = 0;
    //pBurnSoundOut = NULL;
    //audio = NULL;
    nBurnSoundRate = 44100;
#endif
#ifdef __NX__
    nBurnSoundRate = 0;
    nBurnSoundLen = 0;
    pBurnSoundOut = NULL;
    audio = NULL;
#else
    audio = (Audio *) new C2DAudio(nBurnSoundRate, nBurnFPS);
    if (audio->available) {
        nBurnSoundRate = audio->frequency;
        nBurnSoundLen = audio->buffer_len;
        pBurnSoundOut = audio->buffer;
    } else {
        nBurnSoundRate = 0;
        nBurnSoundLen = 0;
        pBurnSoundOut = NULL;
    }
#endif
}

void RunEmulator(Gui *g, int drvnum) {

    printf("RunEmulator '%s'\n", BurnDrvGetTextA(DRV_FULLNAME));

    // set gui pointer
    gui = g;

#if defined(__PSP2__) || defined(__RPI__)
    nSekCpuCore = GetSekCpuCore(gui);
#endif
    bForce60Hz = true;
    nBurnSoundRate = 0;
    if (gui->GetConfig()->GetRomValue(Option::Index::ROM_AUDIO)) {
        nBurnSoundRate = 48000;
    }

    InpInit();
    InpDIP();

    printf("Initialize rom driver\n");
    if (DrvInit(drvnum, false) != 0) {
        printf("Driver initialisation failed! Likely causes are:\n"
                       "- Corrupt/Missing ROM(s)\n"
                       "- I/O Error\n"
                       "- Memory error\n\n");
        DrvExit();
        InpExit();
        return;
    }

    printf("bForce60Hz = %i, nBurnFPS = %d\n", bForce60Hz, nBurnFPS);

    if (nBurnSoundRate > 0) {
        printf("Creating audio device\n");
        AudioInit(gui->GetConfig());
    } else {
        printf("Audio disabled\n");
        audio = NULL;
        nBurnSoundLen = 0;
        pBurnSoundOut = NULL;
    }

    printf("Creating video device\n");
    video = new Video(gui->GetRenderer());

    RunReset();

    int frame_limit = nBurnFPS / 100, frametime = 100000000 / nBurnFPS;
    int now = 0, done = 0, timer = 0, tick = 0, ticks = 0, fps = 0;

    printf("---- PFBA EMU START ----\n\n");

    StartTicks(); // no frameskip

    GameLooping = true;

    // prevent flickering borders by rendering a few frames and
    // blanking the display afterwards
    for (int i = 0; i < 9; i++)
        RunOneFrame(true, 0, 0);
    for (int i = 0; i < 3; i++) {
        video->Clear();
        video->Flip();
    }

    while (GameLooping) {

        int showFps = gui->GetConfig()->GetRomValue(Option::Index::ROM_SHOW_FPS);
        int frameSkip = gui->GetConfig()->GetRomValue(Option::Index::ROM_FRAMESKIP);

        if (frameSkip) {
            timer = GetTicks() / frametime;
            if (timer - tick > frame_limit && showFps) {
                fps = nFramesRendered;
                nFramesRendered = 0;
                tick = timer;
            }
            now = timer;
            ticks = now - done;
            if (ticks < 1) continue;
            if (ticks > 10) ticks = 10;
            for (int i = 0; i < ticks - 1; i++) {
                RunOneFrame(false, showFps, fps);
                if (audio) {
                    audio->Play();
                }
            }
            if (ticks >= 1) {
                RunOneFrame(true, showFps, fps);
                if (audio) {
                    audio->Play();
                }
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
            if (audio) {
                audio->Play();
            }
        }
    }

    printf("---- PFBA EMU END ----\n\n");

    DrvExit();
    InpExit();
    delete (video);
    video = NULL;
    if (audio) {
        delete (audio);
        audio = NULL;
    }
}
