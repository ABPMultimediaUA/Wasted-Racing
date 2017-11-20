#include "QuadTree.h"
#include "../GameObject/GameObject.h"


void QuadTree::init(int max, std::vector<uintptr_t> entityList, int qx0, int qx1, int qy0, int qy1, std::string entityType){

    entities = entityList;

    x0 = qx0;
    x1 = qx1;
    y0 = qy0;
    y1 = qy1;

    maxEntity = max;

    type = entityType;

    if(type=="GameObject"){
        GameObject gameObjects = reinterpret_cast<GameObject>(&entities);
    }
    else if(type=="Component"){
        //habria que hacer lo mismo para cada tipo de entidad con la que se pueda hacer un quadtree???
    }

}

void QuadTree::divide(){

    //splits the quadrant into four equal parts
    if (maxEntity < entities.size()){
        int x01 = (x0 + x1) / 2;
        int y01 = (y0 + y1) / 2;

        //Creation of the 4 children nodes
        std::vector<uintptr_t> aux1;
        QuadTree northWest;
        northWest.init(maxEntity, aux1, x0, x01, y1, y01, type);

        std::vector<uintptr_t> aux2;
        QuadTree northEast;
        northEast.init(maxEntity, aux2, x01, x1, y1, y01, type);

        std::vector<uintptr_t> aux3;
        QuadTree southWest;
        northEast.init(maxEntity, aux3, x0, x01, y01, y1, type);

        std::vector<uintptr_t> aux4;
        QuadTree southEast;
        northEast.init(maxEntity, aux4, x01, x1, y01, y1, type);

        //Inserting the entities into the children nodes
        for (int i = 0; i< entities.size(); i++){//we have to give IComponent a position parameter

            auto position = entities.at(i)->GetTransformable().getPosition(); //ES GENERICAAAA WTF

            if (x0 <= position.x && x01 >=position.x && y0 <= position.y && y01 >= position.y){//if it is inside node northwest
                aux1.entities.push_back(entities.at(i)); //NO NOS VALE PORQUE GENERALIZACION M8
            }
            else if (x01 <= position.x && x1>= position.x && y0 <= position.y && y01 >= position.y){//if it is inside node northeast
                aux2.entities.push_back(entities.at(i));
            }
            else if (x0 <= position.x && x1>= position.x && y01 <= position.y && y1 >= position.y){//if it is inside node southwest
                aux3.entities.push_back(entities.at(i));
            }
            else if (x01 <= position.x && x1>= position.x && y0 >= position.y && y1 >= position.y){//if it is inside node southeast
                aux3.entities.push_back(entities.at(i));
            }

        }

        entities.clear();

        //Inserting the children nodes into the vector
        children.push_back(northWest);
        children.push_back(northEast);
        children.push_back(southWest);
        children.push_back(southEast);

        for (int i = 0; i < children.size(); i++){//We keep dividing the children in case they're full of objects
            children.at(i).split();
        }

    }


}

void QuadTree::update(float dTime, s){//Just for checking collisions????

}



