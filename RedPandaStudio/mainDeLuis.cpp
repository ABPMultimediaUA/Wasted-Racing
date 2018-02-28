#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");
    while(1){
        rps->updateDevice();
        rps::rotateNode(t, 0.01, 1);
    }
    

    getchar();

    return 0;

}