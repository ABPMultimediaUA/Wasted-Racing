#include "src/Facade/RedPandaStudio.h"
#include <iostream>

int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/Link.obj");
    TNode* t3 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "");
    TNode* camera = rps->createCamera(t3->getFather(), glm::vec3(0,0,10));

    rps::scaleNode(t, glm::vec3(0.25,0.25,0.25));

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    bool quit = true;
    SDL_Event event;

    int i = 0;
    float j = 0;

    while( quit ){

        /* Poll for events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               quit = false;
            }
        }

        rps::rotateNode(camera, glm::vec3(0,j,0));
        j+=0.01;
        rps->updateDevice();
        i++;


    }

    rps->dropDevice();


    return 0;

}