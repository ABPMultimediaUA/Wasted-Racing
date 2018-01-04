#pragma once

#include "ISoundEvent.h"

class AcceptEvent : public ISoundEvent{
    public:
        AcceptEvent(FMOD::Studio::EventInstance* newEvent);

};