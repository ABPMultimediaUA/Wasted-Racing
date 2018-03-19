#include "src/Facade/RedPandaStudio.h"
#include <iostream>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "src/Facade/nuklear.h"
#include "src/Facade/nuklear_sdl_gl3.h"

int main() {

    rps::RedPandaStudio* rps = &rps::RedPandaStudio::createDevice(1280,720,24,60,true,false);

    TNode* t = rps->createObjectNode(rps->getSceneRoot(), glm::vec3(0,0,0), "Link/Linkea.obj");
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
        nk_input_begin(ctx);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                nk_sdl_shutdown();
                SDL_Quit();
            }
            nk_sdl_handle_event(&event);
        } nk_input_end(ctx);

        //rps::rotateNode(t, glm::vec3(0,j,0));
        j+=0.01;
        rps->updateDevice();
        i++;


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