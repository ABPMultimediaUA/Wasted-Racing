#pragma once

#include "IComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <vector>

class AIDrivingComponent : public IComponent{
    public:
        AIDrivingComponent(GameObject& newGameObject) : IComponent(newGameObject) {};

        //Destructor
        virtual ~AIDrivingComponent() {};

        //Initilizer
        virtual void init();

        //Update
        virtual void update(float dTime);

        //Closer
        virtual void close();

        void checkList();

        //Makes NPC turn a fixated angle based on data
        static float girar(std::vector<VObject::Pointer> array, std::vector<VObject::Pointer> walls, glm::vec3 waypoint, float a, float b);
        
        //Makes NPC accelerate or brake based on data
        static float acelerar_frenar(std::vector<VObject::Pointer>, float direction, float speed, float a_w, float b_w);

        //Inferes the fuzzy value in a line with the type given
        static float inferL(float value, float limit1, float limit2, int type);

        //Inferes the fuzzy value in a triangular function given the parameters
        static float inferT(float value, float limit1, float limit2, float limit3);

        //Inferes the fuzzy value in a trapeizodal function
        static float inferT2(float value, float limit1, float limit2, float limit3, float limit4);

        //Inferes the fuzzy value with a circular inference
        static float inferC(float value, float limit1, float limit2, float limit3);

        //Returns in Cx and Cy, and the area of the triangle's X,Y positions of its centroid, given the height h
        static void centroidT(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3);

        //Returns in Cx and Cy. and the area of the trapezoid's X,Y positions of its centroid, given the height h
        static void centroidT2(float* cx, float* cy, float* area, float h, float limit1, float limit2, float limit3, float limit4);
    
        //setters
    
    private:
        
};