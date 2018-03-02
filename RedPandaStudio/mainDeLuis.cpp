#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,-2), "Link/Link.obj", "Link/YoungLink_grp.png");
    TNode* t1 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,2), "Link/Link.obj", "Link/YoungLink_grp.png");

    TNode* camera = rps->createCamera(t->getFather(), glm::vec3(-10,0,2));
	rps::rotateNode(camera,-glm::half_pi<float>(),1);

    rps::scaleNode(t, glm::vec3(0.5,0.5,0.5));
    rps::scaleNode(t1, glm::vec3(0.5,0.5,0.5));

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    bool quit = true;
    SDL_Event event;

    while( quit ){

        /* Poll for events */
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                case SDL_QUIT:
                    quit = false;
                    break;
                default:
                    break;
            }
        }

        //rps::rotateNode(t, 0.01, 1);
        //rps::rotateNode(t1, -0.01, 1);
        //rps::scaleNode(t,glm::vec3(0.999,0.999,0.999));
        //rps::scaleNode(t1,glm::vec3(1.001,1.001,1.001));
        //rps::translateNode(t,glm::vec3(0,0,0.01));
        //rps::translateNode(t1,glm::vec3(0,-0.005,0));
        //rps::scaleNode(t, glm::vec3(0.995,0.995,0.995));
        //rps::translateNode(t,glm::vec3(0.01,0,0.01));
        //rps::translateNode(t1,glm::vec3(-0.01,0,-0.01));
        //rps::translateNode(light0,glm::vec3(0,0,-0.02));
        //rps::translateNode(light1,glm::vec3(0,0,0.02));
        rps->updateDevice();

    }

    rps->dropDevice();

    /*
    std::cout << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getChild().size() << std::endl;
    std::cout << t->getChild().size() << std::endl;
    std::cout << std::endl;

    std::cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << " " << std::endl;
    std::cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << " " << std::endl;
    std::cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << " " << std::endl;
    std::cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << " " << std::endl;
    std::cout << std::endl;
    */
    return 0;

}