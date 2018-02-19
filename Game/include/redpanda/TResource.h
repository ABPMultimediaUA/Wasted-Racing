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

        const char* getName()                                          {   return name;    };
        void  setName(const char* n)                                   {   name=n;         };

    protected:
        const char*  name;

        //Maybe a ID variable should be added in order to manage the resources optimally
};