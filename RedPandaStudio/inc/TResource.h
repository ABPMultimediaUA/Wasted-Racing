#pragma once

class TResource{
    public:
        TResource() {};
        ~TResource() {};

        virtual void loadResource() = 0;

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        char* getName()                                          {   return name;    };
        void  setName(char* n)                                   {   name=n;         };

    private:
        char*  name;

        //Maybe a ID variable should be added in order to manage the resources optimally
};