#pragma once 

#include "ISoundEvent.h"

class PlayerEvent : public ISoundEvent {

public: 

    enum Track {
        OnChosen,
        OnGood,
        OnBetter,
        OnBad,
        OnWorse,
        OnCrash,
        OnWin,
        OnLoss,
        OnSpeed
    };

    void setTrack(Track newTrack) { track = newTrack; }
    void setPlayer(int newPlayer) { player = newPlayer; }

    virtual void start() {

        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(characterDescription, &characterInstance));

        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)track);
    }

private:

    Track track;
    int player;

};