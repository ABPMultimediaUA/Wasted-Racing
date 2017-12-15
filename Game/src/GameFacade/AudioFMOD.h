#pragma once 

#include "IAudioFacade.h"
#include <fmod_studio.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <iostream>
#include <string>
#include <glm/ext.hpp>

#include "ISoundEvent.h"
#include "AcceptEvent.h"

class ISoundEvent;

class AcceptEvent;

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

private: 

    std::string banksPath;
    FMOD::Studio::System* system = NULL;
    FMOD::System* lowLevelSystem = NULL;
    //Banks
    FMOD::Studio::Bank* masterBank;
    FMOD::Studio::Bank* stringsBank;
    FMOD::Studio::Bank* menuBank;

    //Events should go on another Fachade
    //Events descriptions
    FMOD::Studio::EventDescription* acceptDescription;

    //Events
    FMOD::Studio::EventInstance* acceptInstance;

    AcceptEvent* acceptEvent;

};
