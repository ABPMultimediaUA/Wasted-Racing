#pragma once

#include <irrlicht.h>
#include <glm/ext.hpp>
#include <iostream>
#include <list> 

class WayPoint{
private:
       
public:

        class Node{
        private:
                float radius;
                glm::vec3 position;
                int level;
                std::list<WayPoint::Node*> *nextNodes;
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
                void addNextNodes(WayPoint::Node* n);
                std::list<WayPoint::Node*> getNextNodes();
                bool checkNext(glm::vec3 pos);
                void clearListNext();
                int getAccess();
                void maxAcces();
        };

        Node* startPoint;

        WayPoint* next;

        std::list<Node*> *listSubNodes;

        float distNextWay = 0;

        Node* auxNode;

        Node* lastNode;
        float distLastNode = -1;
        int lastLevel = 0;

        //Constructor
        WayPoint(glm::vec3 pos, float rad);

        //Destructor
        ~WayPoint();

        glm::vec3 getPos();

        WayPoint::Node* getNode();

        bool checkNext(glm::vec3 pos, glm::vec3 nextPos);
        
        WayPoint* getNext();
        
        void setNext(WayPoint* n);

        float getDistNextWays();

        void addSubNodes(glm::vec3 pos, float rad, int lvl);

        std::list<WayPoint::Node*> getSubNodes();

        WayPoint::Node *getLastNode();

        void setLastNode(Node* n);

        glm::vec3 nextNode();

        void setDistLastNode(Node *n, glm::vec3 pos);

        float getDistLastNode();

        void setLastLevel(int lvl);

        int getLastLevel();


};
