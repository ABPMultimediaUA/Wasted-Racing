#pragma once

#include "Decorator.h"

class Repeat : public Decorator
{
    protected: 
        virtual BehaviourState Update(float d) override;
        
    public:
        Repeat(const Behaviour::Pointer& child) : Decorator(child) {};

};