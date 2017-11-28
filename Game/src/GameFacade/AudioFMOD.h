#pragma once 

#include "IAudioFacade.h"

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
    virtual void playObject(std::string id);

    //Renders all the scene
    virtual void playSound();

private: 

};
