#pragma once

#include "IComponent.h"
#include "GameObject.h"

class IAIComponent : public IComponent {
    public:

        IAIComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

};