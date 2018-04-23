#include "src/Facade/RedPandaStudio.h"
#include <iostream>
#include <GL/glew.h>

#ifdef _WIN32
#define main WinMain 
#endif

int main() {

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "$$\\   $$\\ $$\\   $$\\           $$\\     $$\\                     "         << std::endl;
    std::cout << "$$ |  $$ |$$ | $$  |          $$ |    \\__|                    "              << std::endl;
    std::cout << "\\$$\\ $$  |$$ |$$  / $$$$$$\\ $$$$$$\\   $$\\ $$$$$$$\\   $$$$$$\\  "        << std::endl;
    std::cout << " \\$$$$  / $$$$$  /  \\____$$\\\\_$$  _|  $$ |$$  __$$\\ $$  __$$\\ "         << std::endl;
    std::cout << " $$  $$<  $$  $$<   $$$$$$$ | $$ |    $$ |$$ |  $$ |$$ /  $$ |"               << std::endl;
    std::cout << "$$  /\\$$\\ $$ |\\$$\\ $$  __$$ | $$ |$$\\ $$ |$$ |  $$ |$$ |  $$ |"          << std::endl;
    std::cout << "$$ /  $$ |$$ | \\$$\\\\$$$$$$$ | \\$$$$  |$$ |$$ |  $$ |\\$$$$$$$ |"          << std::endl;
    std::cout << "\\__|  \\__|\\__|  \\__|\\_______|  \\____/ \\__|\\__|  \\__| \\____$$ |"     << std::endl;   
    std::cout << "                                                    $$\\   $$ |"              << std::endl;
    std::cout << "                                                    \\$$$$$$  |"              << std::endl;
    std::cout << "                                                     \\______/ "              << std::endl;

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createAnimatedNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/anim/WitchFINAL_000", false, 60, 0.1f);
    TNode* t2 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/cuboprueba.obj");
    TNode* t3 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "");
    TNode* camera = rps->createCamera(rps->getSceneRoot(), glm::vec3(10,15,0), glm::vec3(0,0,0));

    rps::scaleNode(t, glm::vec3(0.25,0.25,0.25));
    rps::scaleNode(t2, glm::vec3(10,0.25,10));
    rps->setCulling(true, (GLenum)GL_BACK);

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    bool quit = true;
    SDL_Event event;

    float i = 0;
    float j = 0;

    //TAnimation* anim = dynamic_cast<TAnimation*>(t->getEntity());
    //anim->getAnimation()->setLoop(true);

    while( quit ){

        /* Poll for events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               quit = false;
            }            
        }

        
        anim->getAnimation()->update(30);
        rps->updateCamera(glm::vec3(10, 0, 0), glm::vec3(0,0,0));
        rps->updateDevice();
        rps::translateNode(t, glm::vec3(i,i,i));
        i += 0.01;

    }

    rps->dropDevice();

    std::cout << "Thanks for trying RedPandaStudio Engine!" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    return 0;

}
