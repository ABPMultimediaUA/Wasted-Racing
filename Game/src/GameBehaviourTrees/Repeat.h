#pragma once

#include "Decorator.h"
#include <string>
 #include <iostream>

class Repeat : public Decorator
{
    protected: 
        virtual BehaviourState Update(float d) override;
        std::string name = "Repeat";
        
    public:
        Repeat(const Behaviour::Pointer& child) : Decorator(child) {};
        std::string getName() {     return name;    }

};