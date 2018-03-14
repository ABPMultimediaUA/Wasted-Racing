#include "InputRedPanda.h"

void InputRedPanda::openInput(uintptr_t dev) {
    device = reinterpret_cast<SDL_Window*>(dev);
}

void InputRedPanda::updateInput() { 


    // Process events
    SDL_Event event;
    while( SDL_PollEvent( &event ) )
    {
        //Update input 
        DetectKeyInput(SPACE, Key_Jump_Down, Key_Jump_Up)
        DetectKeyInput(ESCAPE,Key_Back_Down,Key_Back_Up)
        DetectKeyInput(w,Key_Advance_Down,Key_Advance_Up)
        DetectKeyInput(s,Key_Brake_Down,Key_Brake_Up)
        DetectKeyInput(a,Key_TurnLeft_Down,Key_TurnLeft_Up)
        DetectKeyInput(d,Key_TurnRight_Down,Key_TurnRight_Up)
        DetectKeyInput(t,Key_Drift_Down,Key_Drift_Up)
        DetectKeyInput(q,Key_UseItem_Down,Key_UseItem_Up)
        DetectKeyInput(5,Key_Multiplayer_Down,Key_Multiplayer_Up)
        DetectKeyInput(6,Key_Singleplayer_Down,Key_Singleplayer_Up)
        DetectKeyInput(F9,Key_DebugAI_Down,Key_DebugAI_Up)
        DetectKeyInput(F10,Key_DebugBehaviour_Down,Key_DebugBehaviour_Up)
        DetectKeyInput(F11,Key_DebugCamera_Down,Key_DebugCamera_Up)

        //Exit game
        if (event.type == SDL_QUIT)
            EventManager::getInstance().addEvent(Event {EventType::Game_Close});
    }
}

void InputRedPanda::closeInput() {
       
} 