#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,32,0,false,false);

    rps->createObjectNode(rps->getSceneRoot(), glm::vec3(1000,10,10), "Link/Link.obj");

    rps->updateDevice();

    getchar();

    return 0;

}