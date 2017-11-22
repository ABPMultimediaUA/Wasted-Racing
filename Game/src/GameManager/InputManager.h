#pragma once

#include "../GameFacade/IInputFacade.h"

class InputManager{

public:

    //Constructor
    InputManager() {}

    //Destructor
    ~InputManager() {
        delete inputFacade;
    }

    //Initializer
    void init(int engine);

    //Updater
    void update();

    //Closer
    void close();

    //Static class getter
    static InputManager& getInstance();

private: 

    IInputFacade* inputFacade;

};