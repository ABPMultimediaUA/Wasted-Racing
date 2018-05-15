#include "TAnimation.h"
#include <iostream>

void TAnimation::beginDraw()
{
    if(animation)
    {
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        animation->draw(pointer);
    }
}


void TAnimation::playNoLoop()
{
    if(!playing)
    {
        pointer = 0;
        playing = true;
    }
}

void TAnimation::update(double eTime)
{
    if(playing)
    {
        elapsedTime += eTime;
        if(elapsedTime > framerate)
        {
            pointer++;
            elapsedTime = 0;
        }
        if(pointer == frames - 2  && loop == false)
        {
            playing = false;
        }
        if(pointer>=frames-1)
        {
            pointer = 0;
        }
    }
}