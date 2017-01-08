//
// Created by cpasjuste on 12/12/16.
//

#include <burner.h>
#include <malloc.h>
#include "audio.h"

Audio::Audio(int freq_id) {

    if (freq_id == 0) {
        pBurnSoundOut = NULL;
        nBurnSoundRate = 0;
        nBurnSoundLen = 0;
        return;
    }

    if (freq_id > 4) {
        freq_id = 4;
    }

    nBurnSoundRate = rate[freq_id];
    nBurnSoundLen = ((nBurnSoundRate * 100) / nBurnFPS);
    size = nBurnSoundLen * channels * 2;
    buffer_fba = (short *) malloc((size_t) size);
    memset(buffer_fba, 0, (size_t) size);
    pBurnSoundOut = buffer_fba;
    enabled = true;

    printf("Audio: rate=%i, buffer=%i\n", nBurnSoundRate, size);
}

void Audio::Pause(int pause) {
    paused = pause;
}

Audio::~Audio() {
    if (enabled) {
        if (buffer_fba != NULL) {
            free(buffer_fba);
            buffer_fba = NULL;
        }
        pBurnSoundOut = NULL;
        nBurnSoundRate = 0;
        nBurnSoundLen = 0;
    }
}
