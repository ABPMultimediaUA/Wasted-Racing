#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

class RampCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};