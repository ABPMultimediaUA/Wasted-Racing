#include "ISoundEvent.h"

//We have to declare somewhere our factory
//std::map<std::string, SoundFactory*> ISoundEvent::factories = std::map<std::string, SoundFactory*>();

//Play Sound once
void ISoundEvent::start()
{
    if(soundInstance!=NULL)
    {
        FMOD_Studio_EventInstance_Start(soundInstance);
    }
}

//Stop sound inmediately 
void ISoundEvent::stop()
{

}

//Pause the sound
void ISoundEvent::pause() 
{

}

//Change sound Volume
void ISoundEvent::setVolume(float vol)
{
    if(vol>=0 && vol<=1)
    {
        FMOD_Studio_EventInstance_SetVolume(soundInstance, vol);
    }
} 

//Change sound Gain
void ISoundEvent::setGain(float gain)
{

}

//Change sound position
void ISoundEvent::setPosition(glm::vec3 pos)
{

}

//Check if sound is being played
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