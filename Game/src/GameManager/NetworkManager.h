#pragma once

class NetworkManager{

public: 

    //Constructor
    NetworkManager() {}

    //Destructor
    ~NetworkManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static NetworkManager& getInstance();

private:

};