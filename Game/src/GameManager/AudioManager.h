#pragma once

#include "../GameFacade/IAudioFacade.h"

class AudioManager{

public: 

    //Constructor
    AudioManager() {}

    //Destructor
    ~AudioManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Play sound
    void play();

    //Shutdown
    void close();

    //Static class getter
    static AudioManager& getInstance();

private:

    IAudioFacade* audioFacade;

};