#pragma once
#include "TEntity.h"

class TCamera : public TEntity {
    public:
        TCamera();
        ~TCamera() {}


        //////////////////////////////////////////////////////////
        //////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////
        bool isParallel()                                          {   return parallel;    };
        void setParallel(bool b)                                   {   parallel=b;         };

        void setCameraParameters(float l, float r, float t, float b, float n, float f);

    private:
        // Definition of the camera
        float left, right, top, bottom, near, far;
        //Parallel or perspective; False == persective, true == parallel
        bool parallel;

};