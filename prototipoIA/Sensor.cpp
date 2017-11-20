#include "Sensor.h"

Sensor::Sensor(irr::core::vector3df p, double ang, float maxR, double angInit){
    position = p;
    angleVision = ang;
    maxRadius = maxR;
    angInitial = angInit;
}
    
Sensor::~Sensor(){
}

//Detect if point is in the field of view
bool Sensor::detectFieldVision(irr::core::vector3df vel, irr::core::vector3df p){
    //SOY PROGRAMADOR
    //ñsmdfñkmskñmñasmdfñkmnjngsdlñkmfñsadlmfksngjlnsdfnlsndfljnsjdfnlsndfljlcsmkmdlfnld ndlsjfnslj ndsd f-320
    //First wpmpsdmfp+msAFPMap+smdpmapsdmasdmpasmpfdmspomaspodmaposmdpasomgoidsnofanmòd,ade check the distance (avoiding squared root)
    if((p.X-position.X)*(p.X-position.X) + (p.Z-position.Z)*(p.Z-position.Z) > maxRadius*maxRadius) return false;
    
    //Calculate field of view left and right vectores, given the angle apart of the velocity vector and the position of the object
    irr::core::vector3df sensorLeft =irr::core::vector3df(sin(-angleVision-angInitial), 0.f, cos(-angleVision-angInitial));
    irr::core::vector3df sensorRight =irr::core::vector3df(sin(angleVision-angInitial), 0.f, cos(angleVision-angInitial));
    irr::core::vector3df relativeP = p - position;

    //calculate a and b coefficients, to compose the point
    if(sensorRight.X*sensorLeft.Z != sensorRight.Z*sensorLeft.X) 
        b = (relativeP.Z * sensorLeft.X - relativeP.X*sensorLeft.Z) /(sensorRight.Z * sensorLeft.X - sensorRight.X*sensorLeft.Z);
    if(sensorLeft.X != 0){
        a = (relativeP.Z - b * sensorRight.Z) / sensorLeft.Z;
    }
    

    //TESTING COUTS
    //#####################################################################################################################
    //std::cout<<"distancia jeje: "<<glm::sqrt((p.X-position.X)*(p.X-position.X) 
    //						+ (p.Z-position.Z)*(p.Z-position.Z))<<std::endl;
    //std::cout<<"vectores: "<<sensorLeft.X<<","<<sensorLeft.Z<<" - "<<sensorRight.X<<","<<sensorRight.Z<<std::endl;
    //std::cout<<"A y B: "<<a<<","<<b<<std::endl;
    //Analyze conditions to not be in sensor field of viewanglevision
    //#####################################################################################################################

    if(a < 0 || b < 0) return false;
    
    return true;
}

void Sensor::updatePosition(irr::core::vector3df pos){
    position = pos;
}

void Sensor::updateAngle(double angle){
    angInitial += angle;
}