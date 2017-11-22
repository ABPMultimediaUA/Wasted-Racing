#pragma once

#include <vector>
#include <list>
#include <cstdint>

//Enumeration of possible events
enum EventType {
    Object_Create,
    Object_Delete,
    Key_Advance,
    Key_Brake,
    Key_IncSpin,
    Key_DecSpin,
    Key_Select,
    Game_Start,
    Game_Close,
    IrrlichtDevice_Start,
    IrrlichtInput_Start
};

//Vector containing a vector with pointers to objects
typedef std::vector<uintptr_t> EventData;

//A event contains a type and the data related to the object
struct Event {

    EventType type;
    EventData data;

};

//An event listener contains a type and a pointer to the funcion to active
struct EventListener{
    EventType listenerType;
    void (*listener)(EventData);
};

//List containig event listeners
typedef std::list<EventListener> EventListenerList;