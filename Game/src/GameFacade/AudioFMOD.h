#pragma once 

#include "IAudioFacade.h"
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <string>
#include <glm/ext.hpp>

#include "../GameAudio/ISoundEvent.h"
#include "../GameAudio/AcceptEvent.h"
#include "../GameAudio/CocodrileGoodENEvent.h"

class ISoundEvent;

class AcceptEvent;

class CocodrileGoodENEvent;

class AudioFMOD : public IAudioFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    AudioFMOD() : IAudioFacade() {}

    //Destructor
    virtual ~AudioFMOD() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates the basic stuff needed on the engine
    virtual void openAudioEngine();

    //Updates the engine
    virtual void update();

    //Closes engine related variables
    virtual void closeAudioEngine();

    //==============================================================
    // Audio Related functions
    //==============================================================

    //Sets the basic volume of the game. Expected value between 0 and 1;
    void setVolume(float vol);

    //Renders all the scene
    virtual void playSound();

    //Sets the 3D position of the listener
    void setListernerPosition(glm::vec3 pos);


    //==============================================================
    // Audio Test Related functions
    //==============================================================

    //Increases the parameter controlled event
    void IncreaseFlanger();

    //Decreases the parameter controlled event
    void DecreaseFlanger();

    //Pauses and unpauses Accept Event
    void stopA();

    //Pauses and unpauses CocodrileGoodENEvent
    void stopK();

private: 

    std::string banksPath;
    FMOD_STUDIO_SYSTEM* system = NULL;
    FMOD_SYSTEM* lowLevelSystem = NULL;
    //Banks
    FMOD_STUDIO_BANK* masterBank;
    FMOD_STUDIO_BANK* stringsBank;
    FMOD_STUDIO_BANK* menuBank;
    FMOD_STUDIO_BANK* cocodrileBank;

    //Events should go on another Fachade
    //Events descriptions
    FMOD_STUDIO_EVENTDESCRIPTION* acceptDescription;
    FMOD_STUDIO_EVENTDESCRIPTION* cocodrileGoodENDescription;

    //Events
    FMOD_STUDIO_EVENTINSTANCE* acceptInstance;
    FMOD_STUDIO_EVENTINSTANCE* cocodrileGoodENInstance;

    AcceptEvent* acceptEvent;
    CocodrileGoodENEvent* cocodrileGoodENEvent;

    bool acceptW;
    bool crocodileW;

};
