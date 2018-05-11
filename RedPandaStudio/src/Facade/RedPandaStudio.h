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
#include <vector>
#include <chrono>

#include <RedPanda.h>

//=========================================================================
//                        RED PANDA STUDIO CLASS                         //
//=========================================================================

namespace rps {

class RedPandaStudio{

public:

    RedPandaStudio() {}
    ~RedPandaStudio() {
        //=========================================================================
        //Delete frame buffers
        glDeleteFramebuffers(1, &depthBuffer);
        //=========================================================================
    }

    //////////////////////////////
    //  DEVICE CONSTRUCTORS
    static RedPandaStudio& createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void updateDevice();
    void dropDevice();

    //////////////////////////////
    //  NODE CONSTRUCTORS

    //Creates an object and returns a TMesh
    TNode* createObjectNode(TNode* parent, glm::vec3 position, const char* mesh); 
    //Creates an animated mesh object and returns a pointer to the TNode that cointains the TAnimation
    TNode* createAnimatedNode(TNode* parent, glm::vec3 pos, const char* animation, bool loop, int frames, double framerate);
    //Creates a camera and returns a TCamera
    TNode* createCamera(TNode* parent, glm::vec3 position, glm::vec3 target);
    //Creates a light and returns a TLight
    TNode* createLight(TNode* parent, glm::vec3 position, glm::vec3 intensity);
    //Creates a spotlight and returns a TSpotlight
    TNode* createSpotlight(TNode* parent, glm::vec3 position, glm::vec3 intensity, glm::vec3 direction, float cutoff);
    //Creates a billboard and returns a reference to it
    TBillboard* createBillboard(char* n, glm::vec3 p);
    //Deletes a mesh, camera or light, given a TMesh, TCamera or TLight
    void deleteObject(TNode* leaf);

    void updateCamera(glm::vec3 position, glm::vec3 target);

    void setGUIDrawFunction(void (*)());

    ////////////////////////////////////////
    //  GRAPHICS OPTIONS AND PARAMETERS
    //Initializes all parameters and programs to operate with the shadow mapping
    void initPostProcessing();

    //Draws the shadow mapping
    void drawPostProcessing();

    //Initializes all parameters and programs to operate with the shadow mapping
    void initShadowMappping();

    //Draws the shadow mapping
    void drawShadowMapping();

    //Activates and deactivates the culling. The second parameter determinates which type of faces are culled (when deactivating the culling, that parameter doesnt matter)
    void setCulling(bool b, GLenum e);

    //To add some mesh into lod array.   
    void addMeshLoD(int lvl, const char* mesh);    

    //////////////////////////////
    //  GETTERS
    SDL_Window* getWindow()                             { return window;           }
    TNode* getSceneRoot()                               { return scene;            }
    ResourceManager* getResourceManager()               { return resourceManager;  }
    SDL_GLContext* getContext()                         { return context;          }  

    //////////////////////////////
    //  SETTERS
    void setWindow(SDL_Window* rw )   {   window = rw;    }   

private: 

    //////////////////////////////
    //  FUNCTIONS
    void initSDLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void initOpenGL();
    void initScene();
    void renderLights();
    void renderCamera();
    void renderBillboards();
    void calculateNodeTransform(TNode* node, glm::mat4& mat);  //Given a node, returns its accumulated transform. Should receive an identity as input
    TNode* addRotScaPos(TNode* parent, glm::vec3 position); //Returns the Position Node
    void deleteNode(TNode* node); //Deletes a node and all his children
    

    //=========================
    //  GENERAL VARIABLES
    //=========================
    SDL_Window* window;
    SDL_GLContext* context;
    TNode *scene;
    ResourceManager *resourceManager;

    //=========================
    //  CAMERA/LIGHTS
    //=========================
    TNode *camera;
    std::vector<TNode*> lights;
    std::vector<TNode*> spotlights;

    //=========================
    //  BILLBOARDS
    //=========================
    //Vector containing all the billboards in the scene
    std::vector<TBillboard*> billboards;
    //Billboard shader
    GLuint billboardID;

    //=========================
    //  SKYBOX
    //=========================
    //Skybox
    TResourceSkybox*  skybox;
    //Skybox shader
    GLuint skyboxID;
    //SKybox vertex array
    GLuint skyVertexArray;

    //=========================
    //  POST-PROCESSING
    //=========================
	GLuint postProcessingBuffer;   //Texture (color) buffer
	GLuint renderBuffer;           //Render buffer ID
	GLuint colorMap;               //Texture in which we paint the scene
    GLuint processingID;           //Shadow map program ID
    GLuint postprocessing_sampler; //Sampler2D of the texture rendered to the quad

    GLuint processingQuadVAO, processingQuadVBO; //Quad indexes
    
    //Window size (used to paint over the quad)
    int windowWidth = 1024;
    int windowHeight = 1024;

    //=========================
    //  SHADOWMAP
    //=========================
	GLuint depthBuffer; //Depth buffer
	GLuint depthMap;    //Texture in which we paint the scene
    GLuint shadowID;    //Shadow map program ID
    GLuint shadowQuadID; //TESTING
	GLuint shadowRenderBuffer;//Render buffer ID
    GLuint shadowQuadVAO, shadowQuadVBO; //Quad indexes
    GLuint shadow_sampler; //Sampler2D of the texture rendered to the quad
    GLuint shadowMap_sampler; //Sampler2D of the depth map in the normal shader

    //Shadow map texture size
    int shadowWidth = 1024;
    int shadowHeight = 1024;

    //=========================
    //  TIME
    //=========================
    //Chrono
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

    //Chrono flag
    bool showFPS = false;
    

};

//Transformation facade
//Input node has to be of type Mesh/Light/Camera
void translateNode(TNode* node, glm::vec3 position);
void rotateNode(TNode* node, glm::vec3 rotation);
void scaleNode(TNode* node, glm::vec3 scale);


}
