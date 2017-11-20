#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <cstdint>
#include <string>

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
        void init(int max, std::vector<uintptr_t> entityList, int qx0, int qx1, int qy0, int qy1, std::string entityType){};

        //Splits a node in 4 and distributes its children between the subnodes if
        //the number of children it's more than the maximum
        void divide(){};

        //Updates a component
        void update(float dTime, std::string type, sf::Vector2f position){};

        //==============================================================
        // Class related Data
        //==============================================================

        //List of entities to insert into the QuadTree
        std::vector<uintptr_t> entities;

        //Children of the quadtree
        std::vector<QuadTree> children;

        //Maximum of entities in each node
        int maxEntity;

        //Quadrants in division
        int x0, x1, y0, y1;

        //Type of the entities in the quadtree
        std::string type;

};

