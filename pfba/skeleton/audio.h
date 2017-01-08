//
// Created by cpasjuste on 12/12/16.
//

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <cstring>

class Audio {

public:

    Audio(int freq_id);
    virtual ~Audio();

    virtual void Play() {};
    virtual void Pause(int pause);

    int rate[5] = {0, 11025, 22050, 44100, 48000};             // Sample rate
    int channels = 2;                                       // Always stereo
    int size = 0;                                           // Seg size in bytes (calculated from Rate/Fps)
    int paused = 0;
    short *buffer_fba = NULL;                               // Buffer where Burn driver will write snd data
    int enabled = 0;
};

#endif //_AUDIO_H_
