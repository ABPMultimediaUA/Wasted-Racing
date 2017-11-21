#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../GameObject/IComponent.h"
#include "../GameObject/GameObject.h"

class QuadTree {

    public:

        //==============================================================
        // Class related Functions
        //==============================================================

        //Constructor
        QuadTree() {};

        //Destructor
        ~QuadTree() {};

        //Initializer
        void init(int max, std::vector<IComponent*> &componentList, int qx0, int qx1, int qy0, int qy1);

        //Splits a node in 4 and distributes its children between the subnodes if
        //the number of children it's more than the maximum
        void divide();

        //Updates a component
        void update(float dTime, glm::vec3 position);

        //Clears all the nodes in the QuadTree
        void clear();

        //==============================================================
        // Class related Data
        //==============================================================

        //List of entities to insert into the QuadTree
        std::vector<IComponent*> *components;

        //Children of the quadtree
        std::vector<QuadTree> nodes;

        //Maximum of entities in each node
        int maxEntity;

        //Quadrants in division
        int x0, x1, y0, y1;

};

