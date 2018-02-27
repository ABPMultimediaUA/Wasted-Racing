#pragma once

//=========================================================================
//                               INCLUDES                                //
//=========================================================================
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <RedPanda.h>

//=========================================================================
//                        RED PANDA STUDIO CLASS                         //
//=========================================================================

namespace rps {

class RedPandaStudio{

public:

    RedPandaStudio() {}
    ~RedPandaStudio() {}

    //////////////////////////////
    //  DEVICE CONSTRUCTORS
    static RedPandaStudio& createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void updateDevice();
    void dropDevice();

    //////////////////////////////
    //  NODE CONSTRUCTORS
    TNode* createObjectNode(TNode* parent, glm::vec3 position, const char* mesh, const char* text);
    TNode* createCamera(TNode* parent, glm::vec3 position);
    TNode* createLight(TNode* parent, glm::vec3 position);

    //////////////////////////////
    //  GETTERS
    SDL_Window* getWindow()   {   return window;      }
    TNode* getSceneRoot()           {   return scene;       }
    ResourceManager* getResourceManager() { return resourceManager;  }

    //////////////////////////////
    //  SETTERS
    void setWindow(SDL_Window* rw )   {   window = rw;    }   

private: 

    //////////////////////////////
    //  FUNCTIONS
    void initSDLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void initOpenGL();
    void initScene();

    //////////////////////////////
    //  VARIABLES
    SDL_Window* window;
    SDL_GLContext context;
    TNode *scene;
    ResourceManager *resourceManager;

};

//Transformation facade
void translateNode(TNode* node, glm::vec3 position);
void rotateNode(TNode* node, float rotation, int axis);
void scaleNode(TNode* node, glm::vec3 scale);


}