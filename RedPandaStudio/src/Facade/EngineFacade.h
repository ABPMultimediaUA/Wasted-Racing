#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include <TNode.h>

class EngineFacade{

public:

    EngineFacade()          {       name = "window";        };
    ~EngineFacade(){};

    void init(TNode* e);
    void update(){};
    void close(){};

    //Setters
    void setWidth(int w)                {       width = w;      }
    void setHeight(int h)               {       height = h;     }
    void setName(std::string n)         {       name = n;       }

    //Getters
    sf::Window* getWindow()             {       return window;          }
    int getWidth()                      {       return width;           }
    int getHeight()                     {       return height;          }


private:

    sf::RenderWindow* window;
    int width, height;
    std::string name;

};