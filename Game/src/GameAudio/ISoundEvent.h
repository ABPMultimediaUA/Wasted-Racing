#pragma once

#include <fmod_studio.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <glm/ext.hpp>
#include <iostream>

class ISoundEvent {
public:

    //Constructor
    ISoundEvent(FMOD_STUDIO_EVENTINSTANCE* newEvent){
        soundInstance = newEvent;
        FMOD_Studio_EventInstance_GetVolume(soundInstance, &volume, 0);
    }

    //Destrcutor 
    virtual ~ISoundEvent() = 0;
    
   //Starts playing the event
    void start();
    
    //Stops inmediatelly to play the event
    void stop();
    
    //Sets the volume of the event
    void setVolume(float vol);
    
    //Sets the gain of the event
    void setGain(float gain);
    
    //Sets the 3D position of the event
    void setPosition(glm::vec3 pos);
    
    //Checks if the event is playing
    bool isPlaying();
    
protected:

    FMOD_STUDIO_EVENTINSTANCE* soundInstance = NULL;

    float volume;

};