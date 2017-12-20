#include "EventManager.h"

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::init() {

}

void EventManager::update() {

    while(!EventManager::eventQueue.empty()){
        EventManager::processEvent(EventManager::eventQueue.front());
        EventManager::eventQueue.pop();
    }

}

void EventManager::close() {
    while(!eventQueue.empty()){
        eventQueue.pop();
    }
    eventListenerMap.clear();
}

void EventManager::addEvent(Event event) {

    EventManager::eventQueue.push(event);

}

void EventManager::processEvent(Event event){

    auto auxEventListenerList = EventManager::eventListenerMap.find(event.type);
    
    if(auxEventListenerList != eventListenerMap.end()) {
        auto eventListenerList = auxEventListenerList->second;

        for(auto eventListener : eventListenerList) {
                eventListener.listener(event.data);
        }
    }

    /* Old version
    auto eventListenerList = EventManager::eventListenerMap.find(event.type)->second;

    for(auto eventListener : eventListenerList) {
        eventListener.listener(event.data);
    }
    */
}

void EventManager::addListener(EventListener eventListener){

    auto iterator = EventManager::eventListenerMap.find(eventListener.listenerType);

    //Check if there's already a list for our listener and if not, create it
    if(iterator==EventManager::eventListenerMap.end()){
        EventListenerList eventListenerList; //new empty list
        iterator = EventManager::eventListenerMap.insert(Pair(eventListener.listenerType, eventListenerList)).first;
    }

    iterator->second.push_back(eventListener);

}