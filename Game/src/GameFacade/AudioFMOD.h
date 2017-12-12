#pragma once 

#include "IAudioFacade.h"
#include <fmod_studio.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <iostream>

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

    //Creates a window depending on the engine
    virtual void openAudioEngine();

    //Updates window info in the engine
    virtual void updateAudioEngine();

    //Closes engine window
    virtual void closeAudioEngine();

    //==============================================================
    // Audio Related functions
    //==============================================================

    //Renders a single object
    virtual void playObject(int id);

    //Renders all the scene
    virtual void playSound();

private: 

    FMOD::Studio::System* system;
    FMOD::System* lowLevelSystem;
    //Banks
    FMOD::Studio::Bank* masterBank;
    FMOD::Studio::Bank* stringsBank;
    FMOD::Studio::Bank* menuBank;

    //Events should go on another Fachade
    //Events descriptions
    FMOD::Studio::EventDescription* acceptDescription;

    //Events
    FMOD::Studio::EventInstance** acceptInstance;
    int i;

};
