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
        //Get components needed for the movement
        auto aiDrivingComponent =  std::dynamic_pointer_cast<AIDrivingComponent>(objectsAI.at(i)).get();
        auto moveComponent = aiDrivingComponent->getGameObject().getComponent<MoveComponent>().get();
        auto vSensorComponent = aiDrivingComponent->getGameObject().getComponent<VSensorComponent>().get();
        auto pathPlanningComponent = aiDrivingComponent->getGameObject().getComponent<PathPlanningComponent>().get();

        //If they all exist
        if(aiDrivingComponent && moveComponent && vSensorComponent){
           
            //get all objects that are seen to the visual sensor
            std::vector<VObject::Pointer> seenObjects = vSensorComponent->getSeenObjects();
            
            //Set angle of the sensor to the NPC one
            vSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
            
            //Get next waypoint
            pathPlanningComponent->setSeconds(1);
            glm::vec3 objective = pathPlanningComponent->getNextPoint(objectsAI.at(i)->getGameObject().getTransformData().position,
                                                            moveComponent->getMovemententData().velocity,
                                                            moveComponent->getMovemententData().vel);


            //Update A and B of the objective
            float a=0,b=0;
            vSensorComponent->calculateAB(objective, &a, &b);

            //DECIDE STUFF
            float turnValue = aiDrivingComponent->girar(seenObjects, objective, a, b);
            float speedValue = aiDrivingComponent->acelerar_frenar(seenObjects, turnValue, vSensorComponent->getAngleInitial(), a, b);
            //----------------------------------

            //------------Testing
            std::cout<<"Turn values: "<<turnValue<<std::endl;
            std::cout<<"Speed value: "<<speedValue<<std::endl;
            //-------------------

            //Send signal of movement
            //Turn
            moveComponent->changeSpin(turnValue);

            //Accelerate and brake
            if(speedValue > 0){
                moveComponent->isMoving(true);
                moveComponent->changeAcc(speedValue);
            }
            if(speedValue < 0){
                moveComponent->isMoving(false);
                moveComponent->changeAcc(speedValue);
            }
            if(speedValue == 0){
                moveComponent->isMoving(false);
            }
           
           
            
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