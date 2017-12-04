#pragma once

#include "IComponent.h"
#include "../GamePhysics/LAPALPhysics.h"

class TerrainComponent : public IComponent {

public:

    //Constructor
	TerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane) : IComponent(newGameObject), terrain(newPlane) {

        next =          nullptr;
        prev =          nullptr;
        leftSibling =   nullptr;
        rightSibling =  nullptr;

    }

	//Destructor
	virtual ~TerrainComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Getters and setters
    TerrainComponent*   getNext()                       {    return next;           }
    TerrainComponent*   getPrev()                       {    return prev;           }
    TerrainComponent*   getLeft()                       {    return leftSibling;    }
    TerrainComponent*   getRight()                      {    return rightSibling;   }
    LAPAL::plane3f      getTerrain()                    {    return terrain;        }
    void                setNext(TerrainComponent* t)    {    next = t;              }
    void                setPrev(TerrainComponent* t)    {    prev = t;              }
    void                setLeft(TerrainComponent* t)    {    leftSibling = t;       }
    void                setRight(TerrainComponent* t)   {    rightSibling = t;      }
    void                setTerrain(LAPAL::plane3f p)    {    terrain = p;           }
    //Connectors
    void connectPrevNext(uint16_t); //receives as input the id of the prev node
    void connectSiblings(uint16_t); //receives as input the id of the left node


private:

    LAPAL::plane3f terrain;

    TerrainComponent* next;
    TerrainComponent* prev;
    TerrainComponent* leftSibling;
    TerrainComponent* rightSibling;

};