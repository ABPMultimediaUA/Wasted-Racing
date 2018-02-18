#include "InputRedPanda.h"

void InputRedPanda::openInput(uintptr_t dev) {

    device = reinterpret_cast<sf::RenderWindow*>(dev);

}

void InputRedPanda::updateInput() { 


    // Process events
    sf::Event event;
    while (device->pollEvent(event))
    {
        //Update input 
        DetectKeyInput(Space, Key_Jump_Down, Key_Jump_Up)
        DetectKeyInput(Escape,Key_Back_Down,Key_Back_Up)
        DetectKeyInput(W,Key_Advance_Down,Key_Advance_Up)
        DetectKeyInput(S,Key_Brake_Down,Key_Brake_Up)
        DetectKeyInput(A,Key_TurnLeft_Down,Key_TurnLeft_Up)
        DetectKeyInput(D,Key_TurnRight_Down,Key_TurnRight_Up)
        DetectKeyInput(T,Key_Drift_Down,Key_Drift_Up)
        DetectKeyInput(Q,Key_UseItem_Down,Key_UseItem_Up)
        DetectKeyInput(Num5,Key_Multiplayer_Down,Key_Multiplayer_Up)
        DetectKeyInput(Num6,Key_Singleplayer_Down,Key_Singleplayer_Up)
        DetectKeyInput(F9,Key_DebugAI_Down,Key_DebugAI_Up)
        DetectKeyInput(F10,Key_DebugBehaviour_Down,Key_DebugBehaviour_Up)
        DetectKeyInput(F11,Key_DebugCamera_Down,Key_DebugCamera_Up)

        //Exit game
        if (event.type == sf::Event::Closed)
            EventManager::getInstance().addEvent(Event {EventType::Game_Close});
    }
}

void InputRedPanda::closeInput() {
       
} 