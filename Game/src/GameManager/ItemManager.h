 #pragma once

#include <vector>
#include "../GameObject/IItemComponent.h"
#include "../GameObject/ItemHolderComponent.h"

 class ItemManager {

    private:

        std::vector<ItemHolderComponent> ItemHolders;
        std::vector<IItemComponent> ItemComponents;

    public:

        ItemManager();

        virtual ~ItemManager();

        virtual void init();

        virtual void update();

        IComponent::Pointer createItemHolderComponent(GameObject& obj);

        static ItemManager& getInstance();

 };
