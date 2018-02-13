#pragma once

class TResource{
    public:
        TResource() {};
        ~TResource() {};

        virtual bool loadResource() = 0;

        virtual void draw() = 0;

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        char* getName()                                          {   return name;    };
        void  setName(char* n)                                   {   name=n;         };

    protected:
        char*  name;

        //Maybe a ID variable should be added in order to manage the resources optimally
};