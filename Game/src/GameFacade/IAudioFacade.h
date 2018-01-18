#pragma once

#include <string>

class IAudioFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IAudioFacade() {}

    //Destructor
    virtual ~IAudioFacade() {}

    //Initializer
    void init() {

    }

    //Closer
    void close() {

    }

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openAudioEngine() = 0;

    //Updates window info in the engine
    virtual void update() = 0;

    //Closes engine window
    virtual void closeAudioEngine() = 0;

protected:

};