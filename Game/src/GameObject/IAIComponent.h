#pragma once

#include "IComponent.h"

class IAIComponent : public IComponent {
    public:

        IAIComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

};