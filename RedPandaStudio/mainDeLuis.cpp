#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,-2), "Link/prueba_cubo.obj");
    TNode* t1 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,2), "Link/prueba_cubo.obj");
    TNode* t2 = rps->createObjectNode(t1->getFather(), glm::vec3(0,0,0), "Link/prueba_cubo.obj");
    TNode* t3 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "");
    TNode* camera = rps->createCamera(t3->getFather(), glm::vec3(0,0,10));

    rps::scaleNode(t, glm::vec3(0.25,0.25,0.25));
    rps::scaleNode(t1, glm::vec3(0.5,0.5,0.5));
    rps::scaleNode(t2, glm::vec3(0.25,0.25,0.25));

    TNode* light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
    TNode* light1 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,-5), glm::vec3(0.01,0.01,0.3));

    bool quit = true;
    SDL_Event event;

    int i = 0;

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



        rps->updateDevice();
        i++;
/*
        if(i % 100 == 0){
            rps->deleteObject(light0);
            light0 = nullptr;
        }
        if(i % 200 == 0){
            light0 = rps->createLight(rps->getSceneRoot(), glm::vec3(0,2,5), glm::vec3(0.3,0.01,0.01));
        }
        if(t1 != nullptr){
            rps::rotateNode(t1, glm::vec3(0,-0.01,0));
            rps::rotateNode(t2, glm::vec3(0,-0.01,0));
        }
        if(i % 50 == 0){
            rps->deleteObject(t1);
            t1 = nullptr;
        }
        if(i % 100 == 0){
            t1 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,2), "Link/Linkea.obj");
            t2 = rps->createObjectNode(t1->getFather(), glm::vec3(0,0,0), "Link/Linkea.obj");
            rps::scaleNode(t1, glm::vec3(0.5,0.5,0.5));
            rps::scaleNode(t2, glm::vec3(0.25,0.25,0.25));
        }
*/

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