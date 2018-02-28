#pragma once

#include "TResource.h"

class TResourceMaterial : public TResource {
    public:
        TResourceMaterial() {};
        ~TResourceMaterial() {};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the material
        void draw();

    private:

};