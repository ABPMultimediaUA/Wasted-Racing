#pragma once

#include "Composite.h"


class Selector : public Composite
{
    protected:
        int s_actualIdx;
        virtual void Initialized() override;
        virtual BehaviourState Update(float d) override;
        
    public: 
        Selector() : s_actualIdx(0) {};

};