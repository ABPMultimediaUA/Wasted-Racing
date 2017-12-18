#include "AIManager.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"
#include <memory>
#include <iostream>

//==============================================
// DELEGATES DECLARATIONS
//==============================================

void addAIDrivingComponent(EventData data);

//==============================================
// AI MANAGER FUNCTIONS
//==============================================

AIManager& AIManager::getInstance() {
    static AIManager instance;
    return instance;
}

void AIManager::init() {
    
    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::AIDrivingComponent_Create, addAIDrivingComponent});
    //No delete by the moment

}
////---------------_______----------------
////---------------MEJORAS----------------
////---------------_______--------------
/*
 >Añadir que la señal sea a través del input manager (checkear performance)
 >Input de los sensores
 >Dividir las llamadas a las funciones en diferentes tempos
 >Implementar con el resource manager
 >Comprobar que no son null y añadir comportamientos externos
*/
//------------------------------------
void AIManager::update() {
    //A update method should be defined by team IA
    for(unsigned int i=0; i<objectsAI.size(); ++i){
        //objectsAI->getNextPoint(glm::vec3 pos, glm::vec3 vel, float modSpeed);
        auto aiDrivingComponent =  std::dynamic_pointer_cast<AIDrivingComponent>(objectsAI.at(i)).get();
        auto moveComponent = aiDrivingComponent->getGameObject().getComponent<MoveComponent>().get();
        auto vSensorComponent = aiDrivingComponent->getGameObject().getComponent<VSensorComponent>().get();

        if(aiDrivingComponent && moveComponent && vSensorComponent){
            std::vector<VObject::Pointer> seenObjects = vSensorComponent->getSeenObjects();
            float a=0,b=0;
            
            vSensorComponent->setAngleInitial(objectsAI[i]->getGameObject().getTransformData().rotation.y);
            
            glm::vec3 sensorLeft = vSensorComponent->getSensorLeft();
            glm::vec3 sensorRight = vSensorComponent->getSensorRight();
            glm::vec3 relativeP;

            std::cout<<"Angle: "<<vSensorComponent->getAngleInitial()<<"\n";
  
            //Get next waypoint
            //aiDrivingComponent->setSeconds(1);
            glm::vec3 objective; /*= aiDrivingComponent->getNextPoint(objectsAI.at(i)->getGameObject().getTransformData().position,
                                                            moveComponent->getMovemententData().velocity,
                                                            moveComponent->getMovemententData().vel);*/


            relativeP = objective - objectsAI.at(i)->getGameObject().getTransformData().position;
            if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
                b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
            if(sensorLeft.x != 0)
                a = (relativeP.z - b * sensorRight.z) / sensorLeft.z;

            //aiDrivingComponent->checkList();
            std::cout<<"Tamaño array: "<<seenObjects.size()<<"\n";
            std::cout<<"Valors A y B: "<<a<<","<<b<<"\n";

            //DECIDE STUFF
            float turnValue = aiDrivingComponent->girar(seenObjects, objective, a, b);
            float speedValue = aiDrivingComponent->acelerar_frenar(seenObjects, turnValue, vSensorComponent->getAngleInitial(), b, a);
            //----------------------------------

            //Send signal of movement
            moveComponent->isMoving(true);
            moveComponent->changeAccInc(speedValue);
            moveComponent->changeSpin(turnValue);
            std::cout<<"DATA: "<<turnValue<<"\n";
        }
        
        //aiDrivingComponent->checkList();
        //std::cout<<"Waypoint: "<<objective.x<<"\n";
        //Make decisions

        //----------------------------------
        //-----------_TESTING_--------------
        //----------------------------------
        //THIS FUNCTION IS OF THE SENSOR; COPIED HERE FOR TESTING USE; COPYRIGHT BIIIIAH
        /*float angleVision = 55*3.14159265358979323846264432737f/180.f;
        float angleInitial =  moveComponent->getMovemententData().angle;
        float a=0,b=0;
        std::vector<VObject*> objects;
        glm::vec3 sensorLeft(sin(-angleVision-angleInitial), 0.f, cos(-angleVision-angleInitial));
        glm::vec3 sensorRight(sin(angleVision-angleInitial), 0.f, cos(angleVision-angleInitial));
        glm::vec3 relativeP;

        relativeP = objective - objectsAI.at(i)->getGameObject().getTransformData().position;
        if(sensorRight.x*sensorLeft.z != sensorRight.z*sensorLeft.x) 
            b = (relativeP.z * sensorLeft.x - relativeP.x*sensorLeft.z) /(sensorRight.z * sensorLeft.x - sensorRight.x*sensorLeft.z);
        if(sensorLeft.x != 0){
            a = (relativeP.z - b * sensorRight.z) / sensorLeft.z;
        }*/

    }
}

void AIManager::close() {

}


//COMPONENT CREATORS
IComponent::Pointer AIManager::createAIDrivingComponent(GameObject& newGameObject){

    IComponent::Pointer component = std::make_shared<AIDrivingComponent>(newGameObject);

    newGameObject.addComponent(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::AIDrivingComponent_Create, data});

    return component;
}


//==============================================
// DELEGATES
//==============================================
void addAIDrivingComponent(EventData data) {
    AIManager::getInstance().getAIDrivingComponentList().push_back(data.Component);
    data.Component.get()->init();
}