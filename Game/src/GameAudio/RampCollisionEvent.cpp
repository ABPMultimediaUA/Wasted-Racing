#include "RampCollisionEvent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

//REGISTER_FACTORY(RampCollisionEvent);


    class RampCollisionEventFactory : public SoundFactory { 
    public: 
        RampCollisionEventFactory() { 
            ISoundEvent::registerFactory(ISoundEvent::getFactoryMap(), "RampCollisionEvent", this); 
        } 
        virtual ISoundEvent* createSound() { 
            return new RampCollisionEvent(); 
        } 
    }; 
    static RampCollisionEventFactory RampCollisionEventFactoryInstance;

void RampCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    glm::vec3 pos = event.CollComponent.get()->getGameObject().getTransformData().position;

    FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
    FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)9);

    FMOD_3D_ATTRIBUTES attributes;
    attributes.position.x = pos.x * audioFMOD->getWorldUnits();
    attributes.position.y = pos.y * audioFMOD->getWorldUnits();
    attributes.position.z = pos.z * audioFMOD->getWorldUnits();

    FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

    FMOD_Studio_EventInstance_Start(soundInstance);

    FMOD_Studio_EventInstance_Release(soundInstance);

}