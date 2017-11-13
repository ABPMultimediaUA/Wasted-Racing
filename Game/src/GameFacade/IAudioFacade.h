#pragma once

#include <string>

class GameObject;

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

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openAudioEngine() = 0;

    //Updates window info in the engine
    virtual void updateAudioEngine() = 0;

    //Closes engine window
    virtual void closeAudioEngine() = 0;

    //==============================================================
    // Audio Related functions
    //==============================================================

    //Renders a single object
    virtual void playObject(std::string id) = 0;

    //Renders all the scene
    virtual void playSound() = 0;

protected:

};