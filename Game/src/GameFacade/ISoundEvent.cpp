#include "ISoundEvent.h"

ISoundEvent::ISoundEvent(FMOD::Studio::EventInstance* newEvent)
{
    soundInstance = newEvent;
    paused=false;
    soundInstance->getVolume(&volume, 0);
    flanger=0.f;
}

ISoundEvent::~ISoundEvent()
{

}


void ISoundEvent::start()
{
    if(soundInstance!=NULL)
    {
        soundInstance->start();
        soundInstance->release();
    }
}

void ISoundEvent::stop()
{

}
    
void ISoundEvent::pause()
{
    paused = !paused;
    soundInstance->setPaused(paused);
}
    
void ISoundEvent::setVolume(float vol)
{
    if(vol>=0 && vol<=1)
    {
        volume=vol;
        soundInstance->setVolume(volume);
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
        soundInstance->getPlaybackState(&state);
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
    soundInstance->setParameterValue(f, flanger);
}

void ISoundEvent::decreaseFlanger()
{   
    char f[10]="Flanger";
    if(flanger>0)
    {
        flanger-=0.1f;
    }
    soundInstance->setParameterValue(f, flanger);
}