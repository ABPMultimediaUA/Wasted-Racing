#include "engineFacade.h"


void EngineFacade::init()
{
    window->create(sf::VideoMode(width, height), "X-Kating");

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
