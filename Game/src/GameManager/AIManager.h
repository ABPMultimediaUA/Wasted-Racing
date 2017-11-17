#pragma once

class AIManager{

public: 

    //Constructor
    AIManager() {}

    //Destructor
    ~AIManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static AIManager& getInstance();

private:

};