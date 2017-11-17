#pragma once


class IGameState {
    
public:
	
    //Constructor
	IGameState() {};

    //Destructor
	virtual ~IGameState() {};

    //Initialization
    virtual void init() = 0;

    //Updater
    virtual void update() = 0;

    //Drawer
    virtual void draw() = 0;

    //Shutdown
    virtual void close() = 0;

protected:

};