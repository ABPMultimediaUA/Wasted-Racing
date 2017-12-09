#include "AIManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

AIManager& AIManager::getInstance() {
    static AIManager instance;
    return instance;
}

void AIManager::init() {

}

void AIManager::update() {
    //A update method should be defined by team IA

}

void AIManager::close() {

}

//==============================================
// DELEGATES
//============================================== 