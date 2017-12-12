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

    std::shared_ptr<TerrainComponent> prevTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        prevTerrain = auxTerrain;
        prevTerrain.get()->setNext(std::shared_ptr<TerrainComponent>(this));   //make the next of previous terrain point to us
        setPrev(prevTerrain);           //make our prev point to the previous terrain
    }
}

void TerrainComponent::connectSiblings(uint16_t id) {

    std::shared_ptr<TerrainComponent> leftTerrain;

    //Get terrain component of the object we want to link with
    auto auxTerrain = ObjectManager::getInstance().getObject(id).get()->getComponent<TerrainComponent>();
    if(auxTerrain!=nullptr){
        leftTerrain = auxTerrain;
        leftTerrain.get()->setNext(std::shared_ptr<TerrainComponent>(this));   //make the next of previous terrain point to us
        setPrev(leftTerrain);           //make our prev point to the previous terrain
    }

}