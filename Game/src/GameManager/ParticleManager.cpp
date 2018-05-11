#include "../GameEvent/EventManager.h"

#include "ParticleManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void createSmokeEvent(EventData eData); 
void deleteParticleEvent(EventData eData);

//==============================================
// PARTICLE MANAGER FUNCTIONS
//============================================== 
ParticleManager& ParticleManager::getInstance() {
    static ParticleManager instance;
    return instance;
}

void ParticleManager::init(){

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::Particles_SmokeON, createSmokeEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Particles_SmokeOFF, deleteParticleEvent});
  
}

void ParticleManager::close(){

    for(unsigned int i = 0; i < emitters.size(); i++) {
        RenderManager::getInstance().getRenderFacade()->deleteObject(emitters[i].id);
    }
    emitters.clear();

}

void ParticleManager::update(){

    for(unsigned int i = 0; i < emitters.size(); i++) {
        GameObject::TransformationData td = (ObjectManager::getInstance().getObject(emitters[i].idToFollow)).get()->getTransformData();
        td.position += emitters[i].offset;
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(emitters[i].id, td);
    }

}

void ParticleManager::createSmokeParticleSystem(uint16_t idToFollow, glm::vec3 offset) {

    GameObject::TransformationData td = (ObjectManager::getInstance().getObject(idToFollow)).get()->getTransformData();
    td.position += offset;

    RenderManager::getInstance().getRenderFacade()->createParticleSystem(lastId, "media/mesh/smoke/smoke.obj", td.position, 2, 200, 5,
                                                                        glm::vec3(0,0.2,0), glm::vec3(0,0.1,0), 0.1,        //Direction
                                                                        0, 2, 0.2,                                          //Size
                                                                        glm::vec4(1,0,0,1), glm::vec4(0,0,1,0.5), 0.5);    //Color

    emitters.push_back(particleSystem{lastId, idToFollow, offset});

    lastId++;

}

void ParticleManager::deleteParticleSystem(uint16_t idToFollow) {

    unsigned int j = -1;

    for(unsigned int i = 0; i < emitters.size(); i++) {
        if(emitters[i].idToFollow == idToFollow) {
            RenderManager::getInstance().getRenderFacade()->deleteObject(emitters[i].id);
            j = i;
        }
    }

    emitters.erase(emitters.begin()+j);

}

//==============================================
// DELEGATES
//==============================================
void createSmokeEvent(EventData eData) {

    ParticleManager::getInstance().createSmokeParticleSystem(eData.Id, eData.Vector);

}
void deleteParticleEvent(EventData eData) {

    ParticleManager::getInstance().deleteParticleSystem(eData.Id);

}