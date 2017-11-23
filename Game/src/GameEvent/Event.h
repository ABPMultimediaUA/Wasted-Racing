#pragma once

#include <vector>
#include <list>
#include <cstdint>

//Enumeration of possible events
enum EventType {
    //Object related events
    Object_Create,
    Object_Delete,
    //PLAYER: Basic control keys
    Key_Advance_Down,
    Key_Advance_Up,
    Key_Brake_Down,
    Key_Brake_Up,
    Key_TurnLeft_Down,
    Key_TurnLeft_Up,
    Key_TurnRight_Down,
    Key_TurnRight_Up,
    //PLAYER: Control keys
    Key_UseItem_Down,
    Key_UseItem_Up,
    Key_Jump_Down,
    Key_Jump_Up,
    Key_Kneel_Down,
    Key_Kneel_Up,
    //PLAYER: Menu control keys
    Key_Select_Down,
    Key_Select_Up,
    Key_Back_Down,
    Key_Back_Up,
    //Game related events
    Game_Start,
    Game_Close
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