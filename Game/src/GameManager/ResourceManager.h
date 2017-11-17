#pragma once

class ResourceManager{

public: 

    //Constructor
    ResourceManager() {}

    //Destructor
    ~ResourceManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static ResourceManager& getInstance();

private:

};