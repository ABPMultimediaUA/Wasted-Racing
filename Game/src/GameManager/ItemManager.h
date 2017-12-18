 #pragma once

#include <vector>
#include "../GameObject/IItemComponent.h"
#include "../GameObject/ItemHolderComponent.h"
#include "ObjectManager.h"
#include "../GameObject/ItemRedShellComponent.h"
#include "../GameObject/ItemBlueShellComponent.h"
#include "../GameObject/ItemBananaComponent.h"
#include "../GameObject/ItemMushroomComponent.h"
#include "../GameObject/ItemStarComponent.h"
#include "../GameObject/ItemBoxComponent.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "../GameObject/ItemHolderComponent.h"


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
        IComponent::Pointer createItem(GameObject::Pointer obj);
        IComponent::Pointer createItemBox(GameObject& obj);

        static ItemManager& getInstance();

        //Item Create
        IComponent::Pointer createRedShell(GameObject::Pointer obj);
        IComponent::Pointer createBlueShell(GameObject::Pointer obj);
        IComponent::Pointer createBanana(GameObject::Pointer obj);
        IComponent::Pointer createMushroom(GameObject::Pointer obj);
        IComponent::Pointer createStar(GameObject::Pointer obj);
        

        //Getters
        std::vector<std::shared_ptr<IItemComponent>> getItemComponents()        {       return ItemComponents;       };


        //Item delete
        void deleteMushroom(IComponent::Pointer component);
        void deleteStar(IComponent::Pointer component);



 };
