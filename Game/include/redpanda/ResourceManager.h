#pragma once

#include <vector>
#include <string.h>
#include <iostream>
#include "TResourceMesh.h"

class ResourceManager {
    public:
        ResourceManager() {};
        ~ResourceManager() {};

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        //Gets the resource from the vector, and if it's not loaded, loads it to the vector and returns him
        TResourceMesh* getResource(char* n);

    private:
        std::vector<TResourceMesh*> resources;

};