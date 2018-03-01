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
#include <vector>

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

    //Creates an object and returns a TMesh
    TNode* createObjectNode(TNode* parent, glm::vec3 position, const char* mesh, const char* text); 
    //Creates a camera and returns a TCamera
    TNode* createCamera(TNode* parent, glm::vec3 position);
    //Creates a light and returns a TLight
    TNode* createLight(TNode* parent, glm::vec3 position);
    //Deletes a mesh, camera or light, given a TMesh, TCamera or TLight
    void deleteObject(TNode* leaf);

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
    //Input node has to be of type Mesh/Light/Camera
void translateNode(TNode* node, glm::vec3 position);
void rotateNode(TNode* node, float rotation, int axis);
void scaleNode(TNode* node, glm::vec3 scale);


}