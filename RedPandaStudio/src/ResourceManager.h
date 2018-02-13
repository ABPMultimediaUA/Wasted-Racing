#pragma once

#include <vector>
#include <string.h>
#include <iostream>
#include "TResource.h"

class ResourceManager {
    public:
        ResourceManager() {};
        ~ResourceManager() {};

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        //Gets the resource from the vector, and if it's not loaded, loads it to the vector and returns him
        TResource* getResource(char* n);

    private:
        std::vector<TResource*> resources;

};