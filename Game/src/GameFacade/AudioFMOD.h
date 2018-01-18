#pragma once 

#include "IAudioFacade.h"
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <string>
#include <map>

#include "../GameAudio/ISoundEvent.h"

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
    virtual void setVolume(float vol);

    //Sets the 3D position of the listener
    virtual void setListenerPosition(glm::vec3 pos);

private: 

    //FMOD SYSTEM
    FMOD_STUDIO_SYSTEM* system = NULL;
    FMOD_SYSTEM* lowLevelSystem = NULL;

    //BANKS
    FMOD_STUDIO_BANK* masterBank;
    FMOD_STUDIO_BANK* stringsBank;
    FMOD_STUDIO_BANK* menuBank;

    std::map<std::string, FMOD_STUDIO_EVENTDESCRIPTION*> eventDescriptions;
    //std::map<std::string, SoundEvent*> soundEvents;

};
