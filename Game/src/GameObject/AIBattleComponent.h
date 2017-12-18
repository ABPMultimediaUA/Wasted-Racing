#pragma once

#include "IComponent.h"
#include "../GameBehaviourTrees/Behaviour.h"
#include "../GameBehaviourTrees/DynamicSelector.h"
#include "../GameBehaviourTrees/Repeat.h"
#include "../GameBehaviourTrees/Selector.h"
#include "../GameBehaviourTrees/Sequence.h"
#include "../GameManager/ItemManager.h"
#include <memory>

class AIBattleComponent : public IComponent {
    public:
        //Constructor
        AIBattleComponent(GameObject& newGameObject) : IComponent(newGameObject) {};

        //Destructor
        virtual ~AIBattleComponent() {};

        //Initilizer
        virtual void init();

        //Update
        virtual void update(float dTime);

        //Closer
        virtual void close();

    private:
        std::shared_ptr<Behaviour> root;

};