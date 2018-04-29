#include "src/Facade/RedPandaStudio.h"
#include <iostream>

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

    //TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/Link.obj");
    //TNode* t2 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "media/cuboprueba.obj");
    TNode* t3 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "");
    TNode* camera = rps->createCamera(rps->getSceneRoot(), glm::vec3(10,2,0), glm::vec3(0,2,0));

    //rps::scaleNode(t, glm::vec3(0.25,0.25,0.25));
    //rps::scaleNode(t2, glm::vec3(10,0.25,10));

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    //Fountain
    TNode* temitter = rps->createEmitter(rps->getSceneRoot(), "media/particle.obj", glm::vec3(0,0,-5), 0, 2000, 4, 
                    glm::vec3(0,0.05,0), glm::vec3(0,-0.01,0), 0.1,
                    0.01, 0.05, 0.5,
                    glm::vec4(0.2,0.2,0.6,1), glm::vec4(1,1,1,1), 0.01);

    //Sparks
    TNode* temitter1 = rps->createEmitter(rps->getSceneRoot(), "media/particle.obj", glm::vec3(0,0,0), 0.01, 200, 0.5, 
                    glm::vec3(0,0.02,0.02), glm::vec3(0,0.04,0.04), 0.1,
                    0.02, 0.01, 0.5,
                    glm::vec4(1,1,0,1), glm::vec4(1,0,0,0.5), 0.1);

    //Smoke
    TNode* temitter2 = rps->createEmitter(rps->getSceneRoot(), "media/particle.obj", glm::vec3(0,0,5), 0, 100, 10, 
                    glm::vec3(0,0.01,0), glm::vec3(0,0.01,0), 0.2,
                    0, 0.5, 0.5,
                    glm::vec4(0.1,0.1,0.1,1), glm::vec4(1,1,1,0.5), 0);

    bool quit = true;
    SDL_Event event;

    float i = 0;

    while( quit ){

        /* Poll for events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               quit = false;
            }            
        }

        //rps->updateCamera(glm::vec3(10*cos(i), 0, 10*sin(i)), glm::vec3(0,0,0));
        rps->updateDevice();
        //rps::translateNode(temitter,glm::vec3(i*10,0, 0));
        i += 0.001;
    }

    rps->dropDevice();

    std::cout << "Thanks for trying RedPandaStudio Engine!" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    return 0;

}
