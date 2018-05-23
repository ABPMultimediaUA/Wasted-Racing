#include "MovementEvents.h"

//===========================================================
//      FACTORY REGISTER
//===========================================================
REGISTER_FACTORY(OnSpeedEvent);
REGISTER_FACTORY(OnSlideEvent);
REGISTER_FACTORY(OnJumpEvent);

//===========================================================
//      ON SPEED EVENT
//===========================================================
void OnSpeedEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnSpeedEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Skate"], &soundInstance);

        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "speed", event.grade);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.25);
    }

}

//===========================================================
//      ON SLIDE EVENT
//===========================================================
void OnSlideEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnSlideEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Slide"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.7);
    }

}

//===========================================================
//      ON JUMP EVENT
//===========================================================
void OnJumpEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnJumpEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Jump"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.7);
    }

}