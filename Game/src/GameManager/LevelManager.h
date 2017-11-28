#pragma once

class LevelManager{

public: 

    //Constructor
    LevelManager() {}

    //Destructor
    ~LevelManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static LevelManager& getInstance();

private:

};