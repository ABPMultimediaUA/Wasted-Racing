#pragma once

#include "IComponent.h"
#include "GameObject.h"

class WaypointComponent : public IComponent {
    private:
        class Node{
        private:
                float radius;
                glm::vec3 position;
        public:                
                Node();
                ~Node();
                float getRadius();
                void setRadius(float rad);
                glm::vec3 getPos();
                void setPos(glm::vec3 pos);
        };
    public:
        Node* startPoint;

        GameObject* next;

        float distNextWay = 0;

        //Constructor
        WaypointComponent(GameObject& newGameObject, glm::vec3 pos, float rad);

        //Destructor
        ~WaypointComponent();

        glm::vec3 getPos();

        bool checkNext(glm::vec3 pos, glm::vec3 nextPos);
        
        GameObject* getNext();
        
        void setNext(GameObject* n);

        float getDistNextWays();
};