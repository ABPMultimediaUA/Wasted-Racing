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

    //==============================================================
    // Audio Related functions
    //==============================================================

    //Renders all the scene
    virtual void playSound() = 0;

    //Increases the flanger of the parameter
    virtual void IncreaseFlanger() = 0;

    //Decreases the flanger of the parameter
    virtual void DecreaseFlanger() = 0;
protected:

};