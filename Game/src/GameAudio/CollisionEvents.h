#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      DEFAULT COLLISION EVENT
//===========================================================
class DefaultCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      RAMP COLLISION EVENT
//===========================================================
class RampCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ITEM BOX COLLISION EVENT
//===========================================================
class ItemBoxCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
<<<<<<< HEAD
//      TRAP COLLISION EVENT
//===========================================================
class TrapCollisionEvent : public ISoundEvent {
=======
//      BANANA COLLISION EVENT
//===========================================================
class BananaCollisionEvent : public ISoundEvent {
>>>>>>> 9417c0d1639e6067b4e72d4c551e3d94986b82ce

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};