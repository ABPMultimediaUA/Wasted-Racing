#pragma once

#include <vector>
#include <list>
#include <cstdint>
#include <memory>

class GameObject;
class IComponent;

//Enumeration of possible events
enum EventType {
    //COMPONENTS: Creation events
    GameObject_Create,
    GameObject_Delete,
    ObjectRenderComponent_Create,
    ObjectRenderComponent_Delete,
    CameraRenderComponent_Create,
    CameraRenderComponent_Delete,
    InputComponent_Create,
    InputComponent_Delete,
    MoveComponent_Create,
    MoveComponent_Delete,
    TerrainComponent_Create,
    TerrainComponent_Delete,
    CollisionComponent_Create,
    CollisionComponent_Delete,
    AIDrivingComponent_Create,
    AIDrivingComponent_Delete,
    RampComponent_Create,
    RampComponent_Delete,
    VSensorComponent_Create,
    VSensorComponent_Delete,
    WaypointComponent_Create,
    WaypointComponent_Delete,
    //COLLISION: Events triggered to certain collisions
    RampComponent_Collision,
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
    //GAME: Game related events
    Game_Start,
    Game_Close
};

//Struct containg diferent types of data
struct EventData {

    std::shared_ptr<GameObject> Object;
    std::shared_ptr<IComponent> Component;
    std::shared_ptr<IComponent> CollComponent;
    float grade;
};

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