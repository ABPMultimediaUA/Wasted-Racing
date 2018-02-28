#include "src/Facade/RedPandaStudio.h"
#include <iostream>


int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");
    TNode* t1 = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(2,0,0), "Link/Link.obj", "Link/YoungLink_grp.png");

    while(true){
        rps::rotateNode(t, 0.01, 1);
        rps::scaleNode(t, glm::vec3(0.999,0.999,0.999));
        rps::translateNode(t,glm::vec3(0.01,0,0.01));
        rps::translateNode(t1,glm::vec3(-0.01,0,-0.01));
        rps->updateDevice();
    }

    /*
    std::cout << t->getFather()->getFather()->getFather()->getFather() << std::endl;
    std::cout << t1->getFather()->getFather()->getFather()->getFather() << std::endl;

    std::cout << t->getFather()->getFather()->getFather() << std::endl;
    std::cout << t1->getFather()->getFather()->getFather() << std::endl;

    std::cout << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t->getFather()->getChild().size() << std::endl;
    std::cout << t->getChild().size() << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << t1->getFather()->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t1->getFather()->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t1->getFather()->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t1->getFather()->getFather()->getChild().size() << std::endl;
    std::cout << t1->getFather()->getChild().size() << std::endl;
    std::cout << t1->getChild().size() << std::endl;
    std::cout << std::endl;

    TTransform* q = (TTransform*)t->getFather()->getEntity();

    std::cout << q->getMatrix()[0][0] << " " << q->getMatrix()[0][1] << " " << q->getMatrix()[0][2] << " " << q->getMatrix()[0][3] << " " << std::endl;
    std::cout << q->getMatrix()[1][0] << " " << q->getMatrix()[1][1] << " " << q->getMatrix()[1][2] << " " << q->getMatrix()[1][3] << " " << std::endl;
    std::cout << q->getMatrix()[2][0] << " " << q->getMatrix()[2][1] << " " << q->getMatrix()[2][2] << " " << q->getMatrix()[2][3] << " " << std::endl;
    std::cout << q->getMatrix()[3][0] << " " << q->getMatrix()[3][1] << " " << q->getMatrix()[3][2] << " " << q->getMatrix()[3][3] << " " << std::endl;
    std::cout << std::endl;
    rps::translateNode(t,glm::vec3(0.01,0,0.01));
    rps->updateDevice();
    std::cout << q->getMatrix()[0][0] << " " << q->getMatrix()[0][1] << " " << q->getMatrix()[0][2] << " " << q->getMatrix()[0][3] << " " << std::endl;
    std::cout << q->getMatrix()[1][0] << " " << q->getMatrix()[1][1] << " " << q->getMatrix()[1][2] << " " << q->getMatrix()[1][3] << " " << std::endl;
    std::cout << q->getMatrix()[2][0] << " " << q->getMatrix()[2][1] << " " << q->getMatrix()[2][2] << " " << q->getMatrix()[2][3] << " " << std::endl;
    std::cout << q->getMatrix()[3][0] << " " << q->getMatrix()[3][1] << " " << q->getMatrix()[3][2] << " " << q->getMatrix()[3][3] << " " << std::endl;
    std::cout << std::endl;
    rps::translateNode(t,glm::vec3(0.01,0,0.01));
    rps->updateDevice();
    std::cout << q->getMatrix()[0][0] << " " << q->getMatrix()[0][1] << " " << q->getMatrix()[0][2] << " " << q->getMatrix()[0][3] << " " << std::endl;
    std::cout << q->getMatrix()[1][0] << " " << q->getMatrix()[1][1] << " " << q->getMatrix()[1][2] << " " << q->getMatrix()[1][3] << " " << std::endl;
    std::cout << q->getMatrix()[2][0] << " " << q->getMatrix()[2][1] << " " << q->getMatrix()[2][2] << " " << q->getMatrix()[2][3] << " " << std::endl;
    std::cout << q->getMatrix()[3][0] << " " << q->getMatrix()[3][1] << " " << q->getMatrix()[3][2] << " " << q->getMatrix()[3][3] << " " << std::endl;
    std::cout << std::endl;
    rps::translateNode(t,glm::vec3(0.01,0,0.01));
    rps->updateDevice();
    std::cout << q->getMatrix()[0][0] << " " << q->getMatrix()[0][1] << " " << q->getMatrix()[0][2] << " " << q->getMatrix()[0][3] << " " << std::endl;
    std::cout << q->getMatrix()[1][0] << " " << q->getMatrix()[1][1] << " " << q->getMatrix()[1][2] << " " << q->getMatrix()[1][3] << " " << std::endl;
    std::cout << q->getMatrix()[2][0] << " " << q->getMatrix()[2][1] << " " << q->getMatrix()[2][2] << " " << q->getMatrix()[2][3] << " " << std::endl;
    std::cout << q->getMatrix()[3][0] << " " << q->getMatrix()[3][1] << " " << q->getMatrix()[3][2] << " " << q->getMatrix()[3][3] << " " << std::endl;
    std::cout << std::endl;
    */
    getchar();

    return 0;

}