#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t2 = rps->getSceneRoot();
    TTransform* t3 = new TTransform();
    t3->identity();
    TNode* t4 = new TNode(t2,t3);
    t2->addChild(t4);

    TNode* t6 = rps->getSceneRoot();
    TTransform* t7 = new TTransform();
    t7->identity();
    TNode* t8 = new TNode(t6,t7);
    t6->addChild(t8);



    TNode* t = rps->createObjectNode(t4, glm::vec3(0,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");
    TNode* t1 = rps->createObjectNode(t6, glm::vec3(2,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");



    
    while(true){
        rps::rotateNode(t, 0.01, 1);
        rps::scaleNode(t, glm::vec3(0.999,0.999,0.999));
        rps::translateNode(t,glm::vec3(0,0,0.01));
        rps::translateNode(t1,glm::vec3(0,0,-0.01));
        rps->updateDevice();
    }
    

    getchar();

    return 0;

}