//
// Created by cpasjuste on 25/11/16.
//

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <cstring>
#include <C2D.h>

class Video {

public:
    Video(Renderer *renderer);
    virtual ~Video();

    virtual void Clear();
    void Lock();
    void Unlock();
    void Render();
    virtual void Flip();
    virtual void Scale();
    virtual void Filter(int filter);

    Renderer *renderer = NULL;
    Texture *screen = NULL;
    Rect scale;
    int VideoBufferWidth = 0;
    int VideoBufferHeight = 0;
    int rotation = 0;
};

#endif //_VIDEO_H_
