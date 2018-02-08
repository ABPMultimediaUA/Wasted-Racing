#pragma once

#include <SFML/Window.hpp>


class EngineFacade{

public:

    EngineFacade(){};
    ~EngineFacade(){};

    void init();
    void update(){};
    void close(){};


private:

    sf::Window* window;
    int width, height;

};