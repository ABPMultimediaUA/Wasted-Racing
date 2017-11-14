#pragma once

#include "IInputFacade.h"

class InputSFML : public IInputFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    InputSFML() : IInputFacade() {}

    //Destructor
    virtual ~InputSFML() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openInput();

    //Updates window info in the engine
    virtual void updateInput();

    //Closes engine window
    virtual void closeInput();

private: 

};