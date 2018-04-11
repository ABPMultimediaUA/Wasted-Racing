#include "RenderRedPanda.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameObject/RenderComponent/LightRenderComponent.h"
#include "../GameObject/RenderComponent/CameraRenderComponent.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_sdl_gl3.h>

//==============================================================
// Gui Related functions and variables declarations
//==============================================================
struct nk_context *GUI; //:::> global variable
void drawRPS_GUI(); //:::> function that is given as parameter to redpanda

namespace gui {

    struct nk_image image;

    void init();
    struct nk_image loadTexture(const char* path);

}

//==============================================================
// Engine Related functions
//==============================================================
//Creates a window depending on the engine
void RenderRedPanda::openWindow() { 
    //Create window with openGL
    device = &rps::RedPandaStudio::createDevice(window.size.x,window.size.y,24,60,window.vsync,window.fullscreen);
    
    //Create input interface
    InputRedPanda* receiver = new InputRedPanda();

    //Get pointer of the window
    uintptr_t aux = reinterpret_cast<uintptr_t>(device->getWindow());

    //Set it as the default device and the input facade as the key handler
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

    //Create HUD renderer
    /*HUDRenderer = SDL_CreateRenderer(device->getWindow(), -1, SDL_RENDERER_ACCELERATED);
    if (HUDRenderer == nullptr){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }

    //Load an image of example
    std::string imagePath = "/media/img/fontcourier.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    }

    //Create texture so we can paint it in the renderer
    tex = SDL_CreateTextureFromSurface(HUDRenderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }*/

    
    //Init GUI
    GUI = nk_sdl_init(device->getWindow());

    //Load fonts
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
	nk_sdl_font_stash_end();

    //set the GUI Context
    receiver->setGUIContext(GUI);

    //set the function that'll draw the GUI
    device->setGUIDrawFunction(drawRPS_GUI);

    //Initialize the GUI
    //gui::init();
}

//Updates window info in the engine
void RenderRedPanda::updateWindow() { 

}

//Closes engine window
void RenderRedPanda::closeWindow() {
    //Clear context
    nk_clear(GUI);
    nk_free(GUI);

    //Close nuklear
    nk_sdl_shutdown();

    //Destroy all textures used -- initially one for testing
    /*SDL_DestroyTexture(tex);

    //Destroy renderer
    SDL_DestroyRenderer(HUDRenderer);*/

    //Close device
    device->dropDevice();

}


//==============================================================
// Render Related functions
//==============================================================
//Renders all the scene
void RenderRedPanda::renderDraw() {
    device->updateDevice();
}

//Add a camera to the game
void RenderRedPanda::addCamera() { }

//Update the current camera
void RenderRedPanda::interpolateCamera(float accTime, float maxTime) { }

//Add an object to the game
void RenderRedPanda::addObject(IComponent* ptr) { 

    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr);

    auto shape = cmp->getObjectShape();
    auto obj = cmp->getGameObject();
    auto pos = obj.getTransformData().position;
    auto sca = obj.getTransformData().scale;
    
    TNode * node = nullptr;
    //Initialize the node
    switch(shape){
        case ObjectRenderComponent::Shape::Mesh: {
            node = device->createObjectNode(device->getSceneRoot(), glm::vec3(0,0,0), cmp->getMesh().c_str());
        }
        break;
        case ObjectRenderComponent::Shape::Cube: {
            node = device->createObjectNode(device->getSceneRoot(), glm::vec3(0,0,0), "media/mesh/box/box.obj");
        }
        break;
        case ObjectRenderComponent::Shape::Plane: {
            node = device->createObjectNode(device->getSceneRoot(), glm::vec3(0,0,0), "media/mesh/box/box.obj");
            rps::scaleNode(node, glm::vec3(sca.x*0.1, sca.y*0.01, sca.z*0.1));
        }
        break;
        default:
        break;
    }

    rps::translateNode(node, glm::vec3(-pos.x, pos.y, pos.z));

    nodeMap.insert(std::pair<uint16_t, TNode*>(obj.getId(), node));

    if(obj.getId() == 25000) {
        device->createCamera(node->getFather(), glm::vec3(0,10,30));
    }

}

//Add an object to the game (Cylinder or Cone)
void RenderRedPanda::addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency) { }

//Add a light to the game
void RenderRedPanda::addLight(IComponent* ptr) {
    
    LightRenderComponent* cmp = dynamic_cast<LightRenderComponent*>(ptr);

    if(cmp != nullptr){

        auto obj = cmp->getGameObject();
        auto type = cmp->getLightType();
        auto pos = obj.getTransformData().position;

        TNode * node;

        //Initialize the node
        switch(type){

            case LightRenderComponent::Type::Point: {
                node = device->createLight(device->getSceneRoot(), pos, glm::vec3(1,1,1));
            }
            break;
            default:
            break;
        }

        nodeMap.insert(std::pair<uint16_t, TNode*>(obj.getId(), node));
    }
 }

//Add an skybox to the game
void RenderRedPanda::addSkybox(IComponent* ptr, std::string t, std::string bo, std::string l, std::string r, std::string f, std::string ba) {
    
}


//Delete an object of the game
void RenderRedPanda::deleteObject(IComponent* ptr) { 

    auto id = ptr->getGameObject().getId();
    auto itr = nodeMap.find(id);

    if(itr != nodeMap.end()){
        auto node = itr->second;
        device->deleteObject(node);
        nodeMap.erase(id);
    }

}

//Change the position of an object in-game
void RenderRedPanda::updateObjectTransform(uint16_t id, GameObject::TransformationData transform) { 

    auto pos = transform.position;
    auto rot = transform.rotation;
    auto sca = transform.scale;

    auto iterator = nodeMap.find(id);
    if(iterator != nodeMap.end()){

        auto node = iterator->second;

            rps::rotateNode(node, glm::vec3(rot.x, -rot.y, rot.z));

            rps::scaleNode(node, sca);

            rps::translateNode(node, glm::vec3(-pos.x, pos.y, pos.z));   
    }
}

//==============================================================
// GUI Related Functions
//==============================================================

//GUI update function
void drawRPS_GUI(){
    
    /*if (nk_begin(GUI, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(GUI, 30, 80, 1);
            if (nk_button_label(GUI, "button"))
                printf("button pressed!\n");
            nk_layout_row_dynamic(GUI, 30, 2);
            if (nk_option_label(GUI, "easy", op == EASY)) op = EASY;
            if (nk_option_label(GUI, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(GUI, 22, 1);
            nk_property_int(GUI, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(GUI, 20, 1);
            nk_label(GUI, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(GUI, 25, 1);

            nk_layout_row_dynamic(GUI, 20, 1);
            nk_label(GUI, "Selected", NK_TEXT_LEFT);
            static const float ratio[] = {0.15f, 0.50f, 0.35f};
            nk_layout_row(GUI, NK_DYNAMIC, 100, 3, ratio);
            nk_spacing(GUI, 1);
            nk_image(GUI, gui::image);

            
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);*/
}

void gui::init() {
    gui::image = gui::loadTexture("media/img/iconoSeta.png");
}

//Code to load a single texture
struct nk_image gui::loadTexture(const char* path) {

    GLuint tex;
    sf::Image sftex;
    sftex.loadFromFile(path);
    const Uint8* data = sftex.getPixelsPtr();

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 500, 500, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    return nk_image_id((int)tex);

}

////////////
//  Image
////////////
//Adds an image on the specific point given with the image specified
int32_t RenderRedPanda::addImage( std::string img, glm::vec2 pos) {return 0;}
//Substitutes an image with another at the same position
void RenderRedPanda::changeImage(int32_t id, std::string img) {}
//Deletes specified rectangle by its index in the rectangle array
void RenderRedPanda::deleteImage(int32_t id) {}
//Clean images off of the screen
void RenderRedPanda::cleanImages() {}

////////////
//  Rectangle
////////////

//Add rectangle of the given color and alpha channel, at the specified position with the given size
int32_t RenderRedPanda::addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a) {return 0;}
//Change color of the rectangle known by the id given
void RenderRedPanda::changeRectangleColor(int32_t id, int r, int g, int b, int a) {}
//Deletes the rectangle with the passed id
void RenderRedPanda::deleteRectangleColor(int32_t id) {}
//Clean all rectangles off of the screen
void RenderRedPanda::cleanRectangles() {} 

////////////
//  Text
////////////
//Adds specified text at the specified point with specified size, with the specified color and font
int32_t RenderRedPanda::addText(std::string text, glm::vec2 pos, int r, int g, int b, int a, glm::vec2 size, std::string) {
    return 0;
}

//Changes the specified text with the given message
void RenderRedPanda::changeText(int32_t id, std::string text) {}
//Changes the font of the game
void RenderRedPanda::changeFontText(int32_t id, std::string font) {}
//Changes the color of the text specified by the id
void RenderRedPanda::changeColorText(int32_t id, int r, int g, int b, int a) {}
//Changes the color of the background of the text specified by the id
void RenderRedPanda::changeBackgroundColorText(int32_t id, int r, int g, int b, int a) {}
//deletes text in the specified position of the text array
void RenderRedPanda::deleteText(int32_t id) {}
//Erase all text from the screen
void RenderRedPanda::cleanTexts() {}
//Erase all visual interface elements from the screen
void RenderRedPanda::cleanInterface() {}

///////////////////////////////
///////      DEBUG      ///////    
///////////////////////////////

void RenderRedPanda::createRectangle2D(glm::vec2 pos, std::string img) { }

void RenderRedPanda::deleteRectangle2D() { }

void RenderRedPanda::createRectangleCol2D(glm::vec2 pos, std::string img) { }

void RenderRedPanda::deleteRectangleCol2D() { }

void RenderRedPanda::createTitleText(glm::vec2 pos, std::string text) { }

void RenderRedPanda::deleteTitleText() { }

void RenderRedPanda::setTitleText(std::string text) { }

void RenderRedPanda::createDescriptionText(glm::vec2 pos, std::string text) { }

void RenderRedPanda::deleteDescriptionText() { }

void RenderRedPanda::setDescriptionText(std::string text) { }

void RenderRedPanda::createSubTitleText(glm::vec2 pos, std::string text) { }

void RenderRedPanda::deleteSubTitleText() { }

void RenderRedPanda::setSubTitleText(std::string text) { }

void RenderRedPanda::createSubDescriptionText(glm::vec2 pos, std::string text) { }

void RenderRedPanda::deleteSubDescriptionText() { }

void RenderRedPanda::setSubDescriptionText(std::string text) { }


//Update the logo video
void RenderRedPanda::updateLogo() { }

void RenderRedPanda::drawGUI() { 
    
}

void RenderRedPanda::createItemIcon(glm::vec2 pos, std::string img) { }

void RenderRedPanda::deleteItemIcon() { }

void RenderRedPanda::updateItemIcon() { }