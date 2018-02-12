#include "engineFacade.h"
#include <iostream>

void EngineFacade::init()
{
    std::cout << "XKating Window: Width -> " << width << " Height -> " << height << std::endl;
    
    window = new sf::RenderWindow(sf::VideoMode(width, height), "XKating");
    
    window->setFramerateLimit(60);

    while(window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->setActive();
        window->display();
    }
} 
