#include "ISoundEvent.h"

ISoundEvent::ISoundEvent(FMOD_STUDIO_EVENTINSTANCE* newEvent)
{
    soundInstance = newEvent;
    FMOD_Studio_EventInstance_GetVolume(soundInstance, &volume, 0);
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