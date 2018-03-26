#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      MUSIC MAIN THEME EVENTS
//===========================================================
class MusicMainThemeEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};