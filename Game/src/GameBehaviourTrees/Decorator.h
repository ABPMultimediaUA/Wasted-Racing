#pragma once

#include "Behaviour.h"
#include <string>
 #include <iostream>

class Decorator : public Behaviour
{
    protected:
        Behaviour::Pointer d_child;
        std::string name = "Decorator";
        
    public:
        Decorator(const Behaviour::Pointer& child) : d_child(child) {};
        void Stop() override;
        void Reset() override;
        std::string getName() {     return name;    }

};
