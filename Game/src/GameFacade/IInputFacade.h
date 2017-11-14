#pragma once

class IInputFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IInputFacade() {}

    //Destructor
    virtual ~IInputFacade() {}

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
    virtual void openInputEngine() = 0;

    //Updates window info in the engine
    virtual void updateInputEngine() = 0;

    //Closes engine window
    virtual void closeInputEngine() = 0;

protected:

};