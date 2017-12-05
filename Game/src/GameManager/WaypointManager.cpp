#include "WaypointManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

WaypointManager& WaypointManager::getInstance() {
    static WaypointManager instance;
    return instance;
}

void WaypointManager::init() {

}

void WaypointManager::update() {
    //I doubt this method should exist in this manager

}

void WaypointManager::close() {

}

//==============================================
// DELEGATES
//============================================== 