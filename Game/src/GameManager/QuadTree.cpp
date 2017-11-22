#include "QuadTree.h"


//@parameter max: the maximum entities inside a node
//@parameter entityList: the list of entities to sort into the quadtree
//@parameters int qx0, qx1, qy0, qy1: the position of the corners of the nodes
//@parameter type: the type of the pointers of the entityList
void QuadTree::init(unsigned int max, std::vector<IComponent*> &componentList, int qx0, int qx1, int qy0, int qy1){

    components = &componentList;

    x0 = qx0;
    x1 = qx1;
    y0 = qy0;
    y1 = qy1;

    maxEntity = max;
    
}

void QuadTree::divide(){

    //splits the quadrant into four equal parts
    if (maxEntity < components->size()){
        int x01 = (x0 + x1) / 2;
        int y01 = (y0 + y1) / 2;

        //Creation of the 4 children nodes
        std::vector<IComponent*> aux1;
        QuadTree northWest;
        northWest.init(maxEntity, aux1, x0, x01, y1, y01);

        std::vector<IComponent*> aux2;
        QuadTree northEast;
        northEast.init(maxEntity, aux2, x01, x1, y1, y01);

        std::vector<IComponent*> aux3;
        QuadTree southWest;
        northEast.init(maxEntity, aux3, x0, x01, y01, y1);

        std::vector<IComponent*> aux4;
        QuadTree southEast;
        northEast.init(maxEntity, aux4, x01, x1, y01, y1);

        //Inserting the entities into the children nodes
        for (unsigned int i = 0; i< components->size(); i++){


            GameObject object = components->at(i)->getGameObject();
            glm::vec3 position = object.getPosition();

            //ARREGLAR LA POSICION Y TODO DONE LALALAL

            if (x0 <= position.x && x01 >=position.x && y0 <= position.y && y01 >= position.y){//if it is inside node northwest
                northWest.components->push_back(components->at(i));
            }
            else if (x01 <= position.x && x1>= position.x && y0 <= position.y && y01 >= position.y){//if it is inside node northeast
                northEast.components->push_back(components->at(i));
            }
            else if (x0 <= position.x && x1>= position.x && y01 <= position.y && y1 >= position.y){//if it is inside node southwest
                southWest.components->push_back(components->at(i));
            }
            else if (x01 <= position.x && x1>= position.x && y0 >= position.y && y1 >= position.y){//if it is inside node southeast
                southEast.components->push_back(components->at(i));
            }

        }

        components->clear();

        //Inserting the children nodes into the vector
        nodes.push_back(northWest);
        nodes.push_back(northEast);
        nodes.push_back(southWest);
        nodes.push_back(southEast);

        for (unsigned int i = 0; i < nodes.size(); i++){//We keep dividing the children in case they're full of objects
            nodes.at(i).divide();
        }

    }
}

//@parameter dTime
//@parameter position: the position of the entity
void QuadTree::update(float dTime, glm::vec3 position){

    int range = 200; //range around the object
    int x01 = (x0 + x1) / 2;
    int y01 = (y0 + y1) / 2;


    for(unsigned int i=0; i<components->size(); i++){
        components->at(i)->update(dTime); //calls the Update function from each
    }

     //We check the entities around the position of our entity to see if they have to update
    if(components->size() != 0){ 
        if(x0-range <= position.x && x01+range >= position.x && y0-range <= position.y && y01+range >= position.y){
            nodes.at(0).update(dTime, position); //updating the entities inside the node northwest
        }
        if(x01-range <= position.x && x1+range >= position.x && y0-range <= position.y && y01+range >= position.y){
            nodes.at(1).update(dTime, position); //updating the entities inside the node northeast
         }
         if(x0-range <= position.x && x01+range >= position.x && y01-range <= position.y && y1+range >= position.y){
            nodes.at(2).update(dTime, position); //updating the entities inside the node southwest
        }
        if(x01-range <= position.x && x1+range >= position.x && y01-range <= position.y && y1+range >= position.y){
            nodes.at(3).update(dTime, position); //updating the entities inside the node southeast
        }
    }
}

void QuadTree::clear(){

    components->clear();

    for(unsigned int i=0; i<nodes.size(); i++){
            nodes.at(i).clear();
    }
}





