#pragma once

class ObjectManager{

public: 

    //Constructor
    ObjectManager() {}

    //Destructor
    ~ObjectManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static ObjectManager& getInstance();

private:

};