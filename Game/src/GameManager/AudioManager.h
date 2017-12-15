#pragma once

#include "../GameFacade/IAudioFacade.h"

class AudioManager{

public: 

    //Constructor
    AudioManager() {}

    //Destructor
    ~AudioManager() {delete audioFacade;}

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

    //Gets Audio Facade
    IAudioFacade* getAudioFacade() {
        return audioFacade;
    }

private:

    IAudioFacade* audioFacade;

};