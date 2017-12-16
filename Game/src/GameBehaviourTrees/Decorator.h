#pragma once

#include "Behaviour.h"

class Decorator : public Behaviour
{
    protected:
        Behaviour::Pointer d_child;
        
    public:
        Decorator(const Behaviour::Pointer& child) : d_child(child) {};
        void Stop() override;
        void Reset() override;

};
