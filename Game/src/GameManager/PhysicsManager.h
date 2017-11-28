#pragma once

class PhysicsManager{

public: 

    //Constructor
    PhysicsManager() {}

    //Destructor
    ~PhysicsManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static PhysicsManager& getInstance();

private:

};