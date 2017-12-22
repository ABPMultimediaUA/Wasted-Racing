 #pragma once

#include <vector>
#include "../GameObject/IItemComponent.h"
#include "../GameObject/ItemHolderComponent.h"
#include "../GameObject/ItemRedShellComponent.h"
#include "../GameObject/ItemBlueShellComponent.h"
#include "../GameObject/ItemBananaComponent.h"
#include "../GameObject/ItemMushroomComponent.h"
#include "../GameObject/ItemStarComponent.h"
#include "../GameObject/ItemBoxComponent.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "ObjectManager.h"


class ItemManager {

    private:

        std::vector<std::shared_ptr<ItemHolderComponent>> ItemHolders;
        std::vector<std::shared_ptr<IItemComponent>> ItemComponents;
        std::vector<std::shared_ptr<ItemBoxComponent>> ItemBoxes;

    public:

        ItemManager();

        ~ItemManager() {};

        void init();

        void update(float dTime);

        void close();

        IComponent::Pointer createItemHolderComponent(GameObject& newGameObject);
        IComponent::Pointer createItem(GameObject& obj);
        IComponent::Pointer createItemBox(GameObject& obj);

        static ItemManager& getInstance();

        //Item Create
        IComponent::Pointer createRedShell(GameObject& obj);
        IComponent::Pointer createBlueShell(GameObject& obj);
        IComponent::Pointer createBanana(GameObject& obj);
        IComponent::Pointer createMushroom(GameObject& obj);
        IComponent::Pointer createStar(GameObject& obj);
        

        //Getters
        std::vector<std::shared_ptr<IItemComponent>>& getItemComponents()        {       return ItemComponents;       };
        std::vector<std::shared_ptr<ItemHolderComponent>>& getItemHolderComponents()        {       return ItemHolders;       };

        //Item delete
        void deleteItem(IComponent::Pointer component);



 };
