/*#include "ISoundEvent.h"
#include <iostream>

ISoundEvent::ISoundEvent(FMOD_STUDIO_EVENTINSTANCE* newEvent)
{
    soundInstance = newEvent;
    paused=false;
    FMOD_Studio_EventInstance_GetVolume(soundInstance, &volume, 0);
    flanger=0.f;
}

ISoundEvent::~ISoundEvent()
{

}


void ISoundEvent::start()
{
    if(soundInstance!=NULL)
    {
        FMOD_Studio_EventInstance_Start(soundInstance);
    }
}

void ISoundEvent::stop()
{

}
    
void ISoundEvent::pause()
{
    paused = !paused;
    FMOD_Studio_EventInstance_SetPaused(soundInstance, paused);
}
    
void ISoundEvent::setVolume(float vol)
{
    if(vol>=0 && vol<=1)
    {
        volume=vol;
        FMOD_Studio_EventInstance_SetVolume(soundInstance, volume);
    }
}
    
void ISoundEvent::setGain(float gain)
{

}
    
void ISoundEvent::setPosition(glm::vec3 pos)
{

}
    
bool ISoundEvent::isPlaying()
{
    if(soundInstance!=NULL)
    {
        FMOD_STUDIO_PLAYBACK_STATE state;
        FMOD_Studio_EventInstance_GetPlaybackState(soundInstance, &state);
        if(state==FMOD_STUDIO_PLAYBACK_PLAYING)
        {
            return true;
        }
    }
    return false;
}

bool ISoundEvent::isPaused()
{
    return paused;
}

void ISoundEvent::increaseFlanger()
{
    char f[10]="Flanger";
    if(flanger<1)
    {
        flanger+=0.1f;
    }
    FMOD_Studio_EventInstance_SetParameterValue(soundInstance, f, flanger);
}

void ISoundEvent::decreaseFlanger()
{   
    char f[10]="Flanger";
    if(flanger>0)
    {
        flanger-=0.1f;
    }
    FMOD_Studio_EventInstance_SetParameterValue(soundInstance, f, flanger);
}
*/