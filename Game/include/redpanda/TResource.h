#pragma once
#include <iostream>

class TResource{
    public:
        TResource() {};
        ~TResource() {
            if(name!=NULL)
                delete name;
        };

        virtual bool loadResource() = 0;

        virtual void draw() = 0;

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        const char* getName()                                          {   return name;    };
        void  setName(const char* n);

    protected:
        const char*  name = NULL;

        //Maybe a ID variable should be added in order to manage the resources optimally
};