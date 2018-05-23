#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      ON SPEED EVENT
//===========================================================
class OnSpeedEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON SLIDE EVENT
//===========================================================
class OnSlideEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON JUMP EVENT
//===========================================================
class OnJumpEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};