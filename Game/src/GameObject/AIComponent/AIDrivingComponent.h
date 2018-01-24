#pragma once

#include "../IComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <vector>
#include "../GameObject.h"
#include "../ItemComponent/ItemHolderComponent.h"

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
        float girar(GameObject& myPos, std::vector<VObject::Pointer> array, std::vector<VObject::Pointer> walls, glm::vec3 waypoint, float a, float b);
        
        //Makes NPC accelerate or brake based on data
        static float acelerar_frenar(GameObject& myPos, std::vector<VObject::Pointer>, float direction, float speed, float a_w, float b_w);

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
    
        //Read AI Values
        void readValues();

        //setters
    
    private:

    //Waypoints Values
    float wp_left_min   = 0,        wp_left_c   = 0,        wp_left_max     = 0;
    float wp_center_min = 0,        wp_center_c = 0,        wp_center_max   = 0;
    float wp_right_min  = 0,        wp_right_c  = 0,        wp_right_max    = 0;

    //Obstacles Values
    float obs_left_min   = 0,        obs_left_c   = 0,        obs_left_max     = 0;
    float obs_center_min = 0,        obs_center_c = 0,        obs_center_max   = 0;
    float obs_right_min  = 0,        obs_right_c  = 0,        obs_right_max    = 0;

    //Item Box Values
    float box_left_min   = 0,        box_left_c   = 0,        box_left_max     = 0;
    float box_center_min = 0,        box_center_c = 0,        box_center_max   = 0;
    float box_right_min  = 0,        box_right_c  = 0,        box_right_max    = 0;

    //Ramp Values
    float rmp_left_min   = 0,        rmp_left_c   = 0,        rmp_left_max     = 0;
    float rmp_center_min = 0,        rmp_center_c = 0,        rmp_center_max   = 0;
    float rmp_right_min  = 0,        rmp_right_c  = 0,        rmp_right_max    = 0;

    //Walls Values
    float wls_left_min   = 0,        wls_left_c   = 0,        wls_left_max     = 0;
    float wls_center_min = 0,        wls_center_c = 0,        wls_center_max   = 0;
    float wls_right_min  = 0,        wls_right_c  = 0,        wls_right_max    = 0;

    //Ratio1 Values
    float wp_left_r1     = 0,        wp_right_r1      = 0;
    float box_left_r1    = 0,        box_right_r1     = 0;

    //Ratio2 Values
    float wp_left_r2     = 0,        wp_right_r2      = 0;
    float rmp_left_r2    = 0,        rmp_right_r2     = 0;

    //Ratio3 Values
    float wp_left_r3     = 0,        wp_right_r3      = 0;
    float box_left_r3    = 0,        box_right_r3     = 0;
    float rmp_left_r3    = 0,        rmp_right_r3     = 0;

    //CentroidT1 Values
    float ctd_left_r1     = 0,      ctd_center_r1   = 0,      ctd_right_r1  = 0;

    //CentroidT2 Values
    float ctd_left_r2     = 0,      ctd_center_r2   = 0,      ctd_right_r2  = 0;

    //CentroidT3 Values
    float ctd_left_r3     = 0,      ctd_center_r3   = 0,      ctd_right_r3  = 0;
        
};