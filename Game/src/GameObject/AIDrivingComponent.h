#pragma once

#include "IAIComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <list>
#include <vector>

class AIDrivingComponent : public IAIComponent{
    private:
        std::list<GameObject*> *listWay;

        GameObject* startPoint;
        GameObject* actualWayPoint;

        int totalWayPoints = 0;
        
        float maxSpeed;

        float frameDeltaTime;

        float seconds;
        
    public:
        AIDrivingComponent(GameObject& newGameObject);

        ~AIDrivingComponent();

        void addWayPoint(GameObject* way);

        void setMaxSpeed(float speed);

        void setFrame(float frame);

        void setSeconds(float sec);

        glm::vec3 getNextPoint(glm::vec3 vel, glm::vec3 pos, float modSpeed);

        //Makes NPC turn a fixated angle based on data
        static float girar(std::vector<VObject*>, glm::vec3 waypoint, float distance, float a, float b, float maxR);
        
        //Makes NPC accelerate or brake based on data
        static float acelerar_frenar(std::vector<VObject*>, glm::vec3 waypoint, float a, float b, float maxR);

        //Inferes the fuzzy value in a line with the type given
        static float inferL(float value, float limit1, float limit2, int type);

        //Inferes the fuzzy value in a triangular function given the parameters
        static float inferT(float value, float limit1, float limit2, float limit3);

        //Inferes the fuzzy value in a trapeizodal function
        static float inferT2(float value, float limit1, float limit2, float limit3, float limit4);

        //Inferes the fuzzy value with a circular inference
        static float inferC(float value, float limit1, float limit2, float limit3);


    

};