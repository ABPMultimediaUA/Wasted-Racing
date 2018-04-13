#include "RenderRedPanda.h"

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

#include "../GameObject/RenderComponent/CameraRenderComponent.h"
#include "../GameObject/RenderComponent/LightRenderComponent.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameManager/RenderManager.h"

//==============================================================
// Gui Related functions and variables declarations
//==============================================================
struct nk_context *GUI; //:::> global variable
void drawRPS_GUI(); //:::> function that is given as parameter to redpanda

namespace gui {

    struct nk_image background;

    void init();
    struct nk_image loadTexture(const char* path);

}

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addHUD(EventData eData); 

//==============================================================
// Engine Related functions
//==============================================================
//Creates a window depending on the engine
void RenderRedPanda::openWindow() { 

    device = &rps::RedPandaStudio::createDevice(window.size.x,window.size.y,24,60,window.vsync,window.fullscreen);

    InputRedPanda* receiver = new InputRedPanda();

    uintptr_t aux = reinterpret_cast<uintptr_t>(device->getWindow());
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

    //Init GUI
    GUI = nk_sdl_init(device->getWindow());
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
	nk_sdl_font_stash_end();

    InputRedPanda* irps = dynamic_cast<InputRedPanda*>(InputManager::getInstance().getInputFacade());
    irps->setGUIContext(GUI);

    gui::init();

    addCamera();

    EventManager::getInstance().addListener(EventListener {EventType::Key_Multiplayer_Down, addHUD});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, addHUD});


}

//Updates window info in the engine
void RenderRedPanda::updateWindow() { 

}

//Closes engine window
void RenderRedPanda::closeWindow() { 

    nk_sdl_shutdown();
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
void RenderRedPanda::addCamera() {

    device->createCamera(device->getSceneRoot(), glm::vec3(10,3,0), glm::vec3(0,0,0));

}

//Update the current camera
void RenderRedPanda::interpolateCamera(float accTime, float maxTime) { 

    //Get target position
    auto pos = cameraTarget->getTransformData().position;

    //Get target y angle
    float radianAngle = cameraTarget->getTransformData().rotation.y;

    //Get interpolated distance to the player
    float oldD = cameraTarget->getComponent<CameraRenderComponent>().get()->getOldDistance();
    float newD = cameraTarget->getComponent<CameraRenderComponent>().get()->getDistance();

    float distance = oldD + (accTime * (newD - oldD))/maxTime;
    distance *= 1.5;

    glm::vec3 target(-pos.x, pos.y, pos.z);
    glm::vec3 position(-pos.x + distance * sin(radianAngle + glm::half_pi<float>()), pos.y + distance * 0.4, pos.z - distance * cos(radianAngle + glm::half_pi<float>()));

    device->updateCamera(position, target);

}

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
void drawRPS_GUI_Menu(){

    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                nk_image(GUI, gui::background);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.35, h*0.25, w*0.3, h*0.5))) {

                nk_layout_row_dynamic(GUI, 50, 1);
                nk_spacing(GUI, 1);
                if (nk_button_label(GUI, "Single Player"))
                    EventManager::getInstance().addEvent(Event {EventType::Key_Singleplayer_Down});
                if (nk_button_label(GUI, "Multiplayer"))
                    EventManager::getInstance().addEvent(Event {EventType::Key_Multiplayer_Down});
                if (nk_button_label(GUI, "Options"))
                    fprintf(stdout, "Options!\n");
                if (nk_button_label(GUI, "Quit"))
                    EventManager::getInstance().addEvent(Event {EventType::Game_Close});
                nk_popup_end(GUI);
            }
            
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_HUD(){

    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x/10, window.size.y/10),0))
        {

            nk_layout_row_static(GUI, window.size.y, window.size.x, 1);

            
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

}

void gui::init() {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();

    device->setGUIDrawFunction(drawRPS_GUI_Menu);

    gui::background = gui::loadTexture("media/img/background.jpg");

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sftex.getSize().x, sftex.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
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

//Delegates
void addHUD(EventData eData) {
    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_HUD);
}