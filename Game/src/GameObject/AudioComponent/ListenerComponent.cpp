#include "ListenerComponent.h"
#include "../../GameManager/AudioManager.h"
#include "../../GameFacade/IAudioFacade.h"

//Initilizer
void ListenerComponent::init() {
    //:::>Shouldn't the manager do this?
    AudioManager::getInstance().getAudioFacade()->setListener(gameObject);
}

//Update
void ListenerComponent::update(float dTime) {

}

//Closer
void ListenerComponent::close() {

}
