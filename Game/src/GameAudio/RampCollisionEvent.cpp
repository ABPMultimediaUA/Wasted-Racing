#include "RampCollisionEvent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

REGISTER_FACTORY(RampCollisionEvent);

void RampCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;

    if(player >= 0 && player <= 4){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)9);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent((ISoundEvent*)this);

    }

}