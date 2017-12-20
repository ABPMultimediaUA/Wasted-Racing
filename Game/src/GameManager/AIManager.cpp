#include "AIManager.h"


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
 >Dejar que otro limpie el codigo
*/
//------------------------------------
void AIManager::update() {
    //Update of all behaviour trees
    for(unsigned int i=0; i<battleAI.size(); i++)
    {
        auto aiBattleComponent = std::dynamic_pointer_cast<AIBattleComponent>(battleAI[i]).get();
        aiBattleComponent->update(0.02f);
    }

    for(unsigned int i=0; i<objectsAI.size(); ++i){
        //Get components needed for the movement
        auto aiDrivingComponent =  std::dynamic_pointer_cast<AIDrivingComponent>(objectsAI.at(i)).get();
        auto moveComponent = aiDrivingComponent->getGameObject().getComponent<MoveComponent>().get();
        auto vSensorComponent = aiDrivingComponent->getGameObject().getComponent<VSensorComponent>().get();
        auto mSensorComponent = aiDrivingComponent->getGameObject().getComponent<MSensorComponent>().get();
        auto pathPlanningComponent = aiDrivingComponent->getGameObject().getComponent<PathPlanningComponent>().get();
        auto iItemComponent = aiDrivingComponent->getGameObject().getComponent<IItemComponent>().get();

        //If they all exist
        if(aiDrivingComponent && moveComponent && vSensorComponent && mSensorComponent && iItemComponent == nullptr){
            //get all objects that are seen to the visual sensor
            std::vector<VObject::Pointer> seenObjects  = vSensorComponent->getSeenObjects();
            std::vector<VObject::Pointer> mapCollisions = mSensorComponent->getMapCollisions();
            for(uint32_t i = 0; i < mapCollisions.size(); ++i){
                seenObjects.push_back(mapCollisions[i]);
            }
            
            //_______________TESTING_______________
            std::cout<<"TAMAÑO LISTA OBSTACULOS: "<<seenObjects.size()<<std::endl;
            //_____________________________________

            //Set angle of the sensors to the NPC one
            vSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
            mSensorComponent->setAngleInitial(moveComponent->getMovemententData().angle);
            
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
            //std::cout<<"Turn values: "<<turnValue<<std::endl;
            //std::cout<<"Speed value: "<<speedValue<<std::endl;
            //-------------------

            //Send signal of movement
            //Turn
            moveComponent->changeSpin(turnValue);

            //Accelerate and brake
            //if(speedValue > 0){
                moveComponent->isMoving(true);
                moveComponent->changeAcc(1.f);
            //}
            /*if(speedValue < 0){
                moveComponent->isMoving(false);
                moveComponent->changeAcc(speedValue);
            }*/
            if(speedValue == 0){
                moveComponent->isMoving(false);
            }
        }
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

IComponent::Pointer AIManager::createAIBattleComponent(GameObject& newGameObject)
{
    IComponent::Pointer component = std::make_shared<AIBattleComponent>(newGameObject);

    newGameObject.addComponent(component);

    battleAI.push_back(component);

   std::dynamic_pointer_cast<AIBattleComponent>(component).get()->init();


    return component;
}


//==============================================
// DELEGATES
//==============================================
void addAIDrivingComponent(EventData data) {
    AIManager::getInstance().getAIDrivingComponentList().push_back(data.Component);
    data.Component.get()->init();
}