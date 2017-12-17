 #pragma once

#include <vector>
#include "../GameObject/IItemComponent.h"
#include "../GameObject/ItemHolderComponent.h"

enum ItemType{
    redShell,      
    blueShell,      
    banana,      
    mushroom,      
    star
};

class ItemManager {

    private:

        std::vector<GameObject::Pointer> ItemHolders;
        std::vector<GameObject::Pointer> ItemComponents;

    public:

        ItemManager();

        ~ItemManager() {};

        void init();

        void update();

        void close();

        IComponent::Pointer createItemHolderComponent(GameObject::Pointer newGameObject);


        static ItemManager& getInstance();

 };
