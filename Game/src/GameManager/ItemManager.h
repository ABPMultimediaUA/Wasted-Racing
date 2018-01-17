 #pragma once

#include <vector>
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/ItemComponent/ItemHolderComponent.h"
#include "../GameObject/ItemComponent/ItemRedShellComponent.h"
#include "../GameObject/ItemComponent/ItemBlueShellComponent.h"
#include "../GameObject/ItemComponent/ItemBananaComponent.h"
#include "../GameObject/ItemComponent/ItemMushroomComponent.h"
#include "../GameObject/ItemComponent/ItemStarComponent.h"
#include "../GameObject/ItemComponent/ItemBoxComponent.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "ObjectManager.h"


class ItemManager {

    private:

        std::vector<std::shared_ptr<ItemHolderComponent>> ItemHolders;
        std::vector<std::shared_ptr<IItemComponent>> ItemComponents;
        std::vector<std::shared_ptr<ItemBoxComponent>> ItemBoxes;


        //Item ids index
        int ids;

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
        std::vector<std::shared_ptr<IItemComponent>>& getItemComponents()                   {       return ItemComponents;       };
        std::vector<std::shared_ptr<ItemHolderComponent>>& getItemHolderComponents()        {       return ItemHolders;       };

        //Item delete
        void deleteItem(IComponent::Pointer component);



 };
