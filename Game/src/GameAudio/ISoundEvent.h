#pragma once

#include <fmod_studio.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <glm/ext.hpp>
#include <iostream>

class ISoundEvent {
public:
    ISoundEvent(FMOD_STUDIO_EVENTINSTANCE* newEvent);
    ~ISoundEvent();
    
   //Starts playing the event
    void start();
    
    //Stops inmediatelly to play the event
    void stop();
    
    //Pauses or unpauses the event, depending on it's current state
    void pause();
    
    //Sets the volume of the event
    void setVolume(float vol);
    
    //Sets the gain of the event
    void setGain(float gain);
    
    //Sets the 3D position of the event
    void setPosition(glm::vec3 pos);
    
    //Checks if the event is playing
    bool isPlaying();

    //Checks if the event is paused. True = paused, false = unpaused
    bool isPaused();

    //Increases and decreases a flanger set as an example parameter
    void increaseFlanger();
    void decreaseFlanger();
    
protected:
    FMOD_STUDIO_EVENTINSTANCE* soundInstance;
    bool paused; //Initially set on false
    float volume;
    float flanger;

    //virtual ISoundEvent* newSoundEvent(FMOD::Studio::EventInstance*) = 0;
};

