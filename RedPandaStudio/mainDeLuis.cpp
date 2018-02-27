#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");
    while(1)
    rps->updateDevice();

    getchar();

    return 0;

}