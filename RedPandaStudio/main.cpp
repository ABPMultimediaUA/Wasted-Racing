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

    //TNode* t = rps->createAnimatedNode(rps->getSceneRoot(), glm::vec3(0,-2,0), "media/anim/WitchFINAL_000", true, 60, 60);
    TNode* t2 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(5,0,1), "media/Link.obj");
    //rps->addMeshLoD(1,"media/mesh/Link/Link.obj");
    TNode* t3 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/cuboprueba.obj");
    TNode* t4 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(40,-1,0), "media/cuboprueba.obj");
    TNode* camera = rps->createCamera(rps->getSceneRoot(), glm::vec3(30,30,5), glm::vec3(0,0,0));

    rps::scaleNode(t2, glm::vec3(0.25,0.25,0.25));
    rps::scaleNode(t4, glm::vec3(100,0.05,100));
    rps->setCulling(true, (GLenum)GL_BACK);

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(5,2,2), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    bool quit = true;
    SDL_Event event;

    float i = 0;
    float j = 0;

    //TAnimation* anim = dynamic_cast<TAnimation*>(t->getEntity());
    //anim->setLoop(true);

    while( quit ){

        /* Poll for events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               quit = false;
            }            
        }

        
        //anim->update(30);
        rps->updateCamera(glm::vec3(15*glm::cos(i), 15, 15*glm::sin(i)), glm::vec3(0,0,0));
        rps->updateDevice();
        rps::translateNode(t2, glm::vec3(0, 0, 5));
        i += 0.01;
        /*auto e = rps->getResourceManager()->getResourceLoD("media/mesh/Link/Link.obj");
        std::cout<<e->getMap().size()<<"\n";*/
    }

    rps->dropDevice();

    std::cout << "Thanks for trying RedPandaStudio Engine!" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    return 0;

}
