#pragma once

#include "Composite.h"

class DynamicSelector : public Composite
{
    protected:
        int actualIdx;    
        
    public:
        virtual void Initialized() override;

        virtual BehaviourState Update(float d) override;

};
