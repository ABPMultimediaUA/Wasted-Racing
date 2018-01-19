#pragma once

class IGameState {
    
public:
    //Type of states
    enum stateType {
        INTRO,          //Game intro (initial state)
        MAIN,           //Game main menu
        MATCH           //inside the game state
    };

    //Constructor
	IGameState(){};

    //Destructor
	virtual ~IGameState() {};

    //Initialization
    virtual void init() = 0;

    //Updater
    virtual void update(float &accumulatedTime) = 0;

    //Drawer
    virtual void draw() = 0;

    //Shutdown
    virtual void close() = 0;

    stateType type;     //Type of state
};
