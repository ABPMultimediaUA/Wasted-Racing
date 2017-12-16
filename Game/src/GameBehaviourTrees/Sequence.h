#pragma once

#include "Composite.h"

class Sequence : public Composite
{
    public:
        Sequence() : s_currentIdx(0) {}

    protected: 
        int s_currentIdx;
        virtual void OnInitialized();
        virtual BehaviourState OnUpdate(float d);      
};
