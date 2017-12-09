#include "SensorManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

SensorManager& SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::init() {

}

void SensorManager::update() {
    //This should call every time to all the sensors to update them.
    //Actually we dont have Environment, so we can't get the objects
    //from the map to be seen

}

void SensorManager::close() {

}

//==============================================
// DELEGATES
//============================================== 