#pragma once

#include "IComponent.h"
#include "GameObject.h"
#include <glm/ext.hpp>
#include <list> 

class WaypointComponent : public IComponent{
private:
       
public:

        class Node{
        private:
                float radius;
                glm::vec3 position;
                int level;
                std::list<WaypointComponent::Node*> *nextNodes;
                int access = 0;
        public:                
                Node(glm::vec3 pos, float rad, int lvl);
                ~Node();
                float getRadius();
                void setRadius(float rad);
                glm::vec3 getPos();
                void setPos(glm::vec3 pos);
                int getLevel();
                void setLevel(int lvl);
                void addNextNodes(WaypointComponent::Node* n);
                std::list<WaypointComponent::Node*> getNextNodes();
                bool checkNext(glm::vec3 pos);
                void clearListNext();
                int getAccess();
                void maxAcces();
        };

        Node* startPoint;

        GameObject* next;

        std::list<Node*> *listSubNodes;

        float distNextWay = 0;

        Node* auxNode;

        Node* lastNode;
        float distLastNode = -1;
        int lastLevel = 0;

        //Constructor
        WaypointComponent(GameObject& newGameObject, glm::vec3 pos, float rad);

        //Destructor
        ~WaypointComponent();

        glm::vec3 getPos();

        WaypointComponent::Node* getNode();

        bool checkNext(glm::vec3 pos, glm::vec3 nextPos);
        
        GameObject* getNext();
        
        void setNext(GameObject* n);

        float getDistNextWays();

        void addSubNodes(glm::vec3 pos, float rad, int lvl);

        std::list<WaypointComponent::Node*> getSubNodes();

        WaypointComponent::Node *getLastNode();

        void setLastNode(Node* n);

        glm::vec3 nextNode();

        void setDistLastNode(Node *n, glm::vec3 pos);

        float getDistLastNode();

        void setLastLevel(int lvl);

        int getLastLevel();

        //Initilizer
	virtual void init() {}

	//Update
	virtual void update(float dTime) {}

	//Closer
	virtual void close() {}

};