#include "TerrainComponent.h"
#include "../GameManager/ObjectManager.h"

//Initilizer
void TerrainComponent::init() {

}

//Update
void TerrainComponent::update(float dTime) {



}

//Closer
void TerrainComponent::close() {

} 




//Connectors
void TerrainComponent::connectPrevNext(uint16_t id) {

    TerrainComponent* prevTerrain = nullptr;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr)
        prevTerrain = auxTerrain.get();

    prevTerrain->setNext(this); //make the next of previous terrain point to us

    setPrev(prevTerrain);       //make our prev point to the previous terrain
    
}
void TerrainComponent::connectSiblings(uint16_t id) {

    TerrainComponent* leftTerrain = nullptr;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr)
        leftTerrain = auxTerrain.get();

    leftTerrain->setRight(this); //make the right of left terrain point to us

    setLeft(leftTerrain);       //make our left point to the left terrain

}