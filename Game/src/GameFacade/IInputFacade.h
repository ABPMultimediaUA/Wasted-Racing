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
    virtual void openInput() = 0;

    //Updates window info in the engine
    virtual void updateInput() = 0;

    //Closes engine window
    virtual void closeInput() = 0;

protected:

};