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
#include "../GameManager/ObjectManager.h"
#include "../GlobalVariables.h"
#include "../GameState/MatchState.h"

//==============================================================
// Gui Related functions and variables declarations
//==============================================================
struct nk_context *GUI; //:::> global variable
void drawRPS_GUI_Menu(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_HUD(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_Options(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_PlayerSelect(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_Pause(); //:::> function that is given as parameter to redpanda

namespace gui {

    //MAIN  MENU Images
    struct nk_image background;
    struct nk_image menuBase;
    struct nk_image text_singleplayer;
    struct nk_image text_singleplayerHover;
    struct nk_image text_multiplayer;
    struct nk_image text_multiplayerHover;
    struct nk_image text_options;
    struct nk_image text_optionsHover;
    struct nk_image text_exit;
    struct nk_image text_exitHover;

    //OPTION MENU Images
    struct nk_image optionsBase;
    struct nk_image text_language;
    struct nk_image text_languageHover;
    struct nk_image text_soundON;
    struct nk_image text_soundONHover;
    struct nk_image text_soundOFF;
    struct nk_image text_soundOFFHover;
    struct nk_image text_volume;
    struct nk_image text_volumeHover;
    struct nk_image text_oexit;
    struct nk_image text_oexitHover;

    //GUI Images
    struct nk_image item_void;
    struct nk_image item_banana;
    struct nk_image item_blue;
    struct nk_image item_red;
    struct nk_image item_star;
    struct nk_image item_mushroom;
    struct nk_image number_1;
    struct nk_image number_2;
    struct nk_image number_3;
    struct nk_image number_4;
    struct nk_image minimap;
    struct nk_image dot_player;
    struct nk_image dot_enemy;
    struct nk_image lap_1;
    struct nk_image lap_2;
    struct nk_image lap_3;

    //PAUSE Images
    struct nk_image pbackground;
    struct nk_image pauseBase;
    struct nk_image text_resume;
    struct nk_image text_resumeHover;
    struct nk_image text_poptions;
    struct nk_image text_poptionsHover;
    struct nk_image text_menu;
    struct nk_image text_menuHover;
    struct nk_image text_pexit;
    struct nk_image text_pexitHover;

    void init();
    struct nk_image loadTexture(const char* path);

    std::vector<GLuint> textures;

}

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addHUD(EventData eData); 
void addPause(EventData eData); 
void changeLanguage(EventData eData);

//==============================================================
// Engine Related functions
//==============================================================
//Creates a window depending on the engine
void RenderRedPanda::openWindow() { 

    window.fullscreen = false;

    device = &rps::RedPandaStudio::createDevice(window.size.x,window.size.y,24,60,window.vsync,window.fullscreen);
    InputRedPanda* receiver = new InputRedPanda();

    if(window.fullscreen){
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        window.size.x = DM.w;
        window.size.y = DM.h;
    }

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

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_Menu);

    addCamera();

    EventManager::getInstance().addListener(EventListener {EventType::Key_Multiplayer_Down, addHUD});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Singleplayer_Down, addHUD});
    EventManager::getInstance().addListener(EventListener {EventType::Global_ChangeLanguage, changeLanguage});
    EventManager::getInstance().addListener(EventListener {EventType::Game_Pause, addPause});


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

//Particles
void RenderRedPanda::createParticleSystem(uint16_t id, const char* shape, glm::vec3 position, float radius, int birthrate, float particleLife,
                                            glm::vec3 birthDirection, glm::vec3 deathDirection, float variationDirection,
                                            float birthSize, float deathSize, float variationSize,
                                            glm::vec4 birthColor, glm::vec4 deathColor, float variationColor) {

    TNode * node = device->createEmitter(device->getSceneRoot(), shape, position, radius, birthrate, particleLife,
            birthDirection, deathDirection, variationDirection, birthSize, deathSize, 
            variationSize, birthColor, deathColor, variationColor);

    rps::translateNode(node, glm::vec3(-position.x, position.y, position.z));

    nodeMap.insert(std::pair<uint16_t, TNode*>(id, node));

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
void RenderRedPanda::deleteObject(uint16_t id) {

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

//Change mesh
bool RenderRedPanda::changeMesh(int id, std::string newMesh)
{
    auto node = nodeMap.find(id)->second;
    auto resourceManager = device->getResourceManager();
    auto resourceObj = resourceManager->getResourceOBJ(newMesh.c_str());
    if(resourceObj != nullptr && resourceObj->getMeshes().size() > 0)
    {
        auto tEntity = node->getEntity();
        auto tMesh = dynamic_cast<TMesh*>(tEntity);
        tMesh->setMesh(resourceObj);
        return true;
    }
    return false;
}

//==============================================================
// GUI Related Functions
//==============================================================

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

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.16, 0, w, h))) {
                nk_layout_row_static(GUI, h, h*1.16, 1);
                nk_image(GUI, gui::menuBase);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.35, h*0.35, w*0.3, h*0.5))) {

                nk_layout_row_dynamic(GUI, h*0.07, 1);
                nk_spacing(GUI, 1);
                if (nk_button_image(GUI, gui::text_singleplayer, gui::text_singleplayerHover))
                    EventManager::getInstance().addEvent(Event {EventType::Key_Singleplayer_Down});
                if (nk_button_image(GUI, gui::text_multiplayer, gui::text_multiplayerHover))
                    EventManager::getInstance().addEvent(Event {EventType::Key_Multiplayer_Down});
                if (nk_button_image(GUI, gui::text_options, gui::text_optionsHover)){
                    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
                    device->setGUIDrawFunction(drawRPS_GUI_Options);
                }
                if (nk_button_image(GUI, gui::text_exit, gui::text_exitHover))
                    EventManager::getInstance().addEvent(Event {EventType::Game_Close});
                nk_popup_end(GUI);
            }
            
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_Options(){

    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::INTRO)
                    nk_image(GUI, gui::background);
                else if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::PAUSE)
                    nk_image(GUI, gui::pbackground);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(h*0.1388, 0, w, h))) {
                nk_layout_row_static(GUI, h, h*1.5, 1);
                nk_image(GUI, gui::optionsBase);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.135, h*0.107, w*0.25, w*0.04))) {
                nk_layout_row_static(GUI, w*0.04, w*0.25, 1);
                if (nk_button_image(GUI, gui::text_language, gui::text_languageHover)){

                    int lang = GlobalVariables::getInstance().getLanguage();
                    if(lang == 0)       lang = 1;
                    else if (lang == 1) lang = 0;

                    EventData eData;
                    eData.Id = lang;
                    EventManager::getInstance().addEvent(Event {EventType::Global_ChangeLanguage, eData});

                    GlobalVariables::getInstance().setLanguage(lang);
                }                
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.705, h*0.81, w*0.15, h*0.15))) {
                nk_layout_row_static(GUI, h*0.15, w*0.15, 1);
                if (nk_button_image(GUI, gui::text_oexit, gui::text_oexitHover)){
                    if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::INTRO){
                        rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
                        device->setGUIDrawFunction(drawRPS_GUI_Menu);
                    }
                    else if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::PAUSE){
                        rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
                        device->setGUIDrawFunction(drawRPS_GUI_Pause);
                    }
                }                
                nk_popup_end(GUI);
            }
            
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_HUD(){

    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;

    GameObject cameraTarget = RenderManager::getInstance().getRenderFacade()->getCameraTarget();

    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0)) {

        if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.0039, h*0.007, w*0.13, h*0.25))) {

            nk_layout_row_static(GUI, h*0.208, w*0.117, 1);

            int itemID = cameraTarget.getComponent<ItemHolderComponent>().get()->getItemType();
            switch(itemID){
                case 0: //RED SHELL
                        nk_image(GUI, gui::item_red);
                        break;
                case 1: //BLUE SHELL
                        nk_image(GUI, gui::item_blue);
                        break;
                case 2: //TRAP
                        nk_image(GUI, gui::item_banana);
                        break;
                case 3: //MUSHROOM
                        nk_image(GUI, gui::item_mushroom);
                        break;
                case 4: //STAR
                        nk_image(GUI, gui::item_star);
                        break;
                default:
                        nk_image(GUI, gui::item_void);
                        break;
            }
            nk_popup_end(GUI);
	    }

        if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.0039, h*0.77, w*0.13, h*0.25))) {

            nk_layout_row_static(GUI, h*0.208, w*0.117, 1);

            int position = cameraTarget.getComponent<ScoreComponent>().get()->getPosition();
            switch(position){
                case 1: //BLUE SHELL
                        nk_image(GUI, gui::number_1);
                        break;
                case 2: //TRAP
                        nk_image(GUI, gui::number_2);
                        break;
                case 3: //MUSHROOM
                        nk_image(GUI, gui::number_3);
                        break;
                case 4: //STAR
                        nk_image(GUI, gui::number_4);
                        break;
                default:
                        break;
            }
            nk_popup_end(GUI);
	    }

        if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.885, h*0.007, w*0.1, h*0.2))) {

            nk_layout_row_static(GUI, h*0.175, w*0.084, 1);

            int lap = cameraTarget.getComponent<ScoreComponent>().get()->getLap();

            switch(lap){
                case 1:
                        nk_image(GUI, gui::lap_1);
                        break;
                case 2:
                        nk_image(GUI, gui::lap_2);
                        break;
                case 3:
                        nk_image(GUI, gui::lap_3);
                        break;
                default:
                        break;
            }
            
            nk_popup_end(GUI);
	    }

        if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.8, h*0.48, w*0.2, h*0.5))) {

            nk_layout_row_static(GUI, h*0.45278, w*0.1586, 1);

            nk_image(GUI, gui::minimap);
            
            nk_popup_end(GUI);
	    }

        for(int i = 3; i >= 0; i--) {

            glm::vec3 pos1 = ObjectManager::getInstance().getObject(25000+i).get()->getTransformData().position;

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.89 - pos1.z * h * 0.00021, h*0.6111 - pos1.x * h * 0.00021, w*0.2, h*0.5))) {

                nk_layout_row_static(GUI, h*0.021, h*0.021, 1);

                if(i == 0)
                    nk_image(GUI, gui::dot_player);
                else 
                    nk_image(GUI, gui::dot_enemy);

                nk_popup_end(GUI);
	        }
        }
    }

	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

}

void drawRPS_GUI_Pause(){
    
    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;

        if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                nk_image(GUI, gui::pbackground);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.29, 0, w, h))) {
                nk_layout_row_static(GUI, h, h*0.75, 1);
                nk_image(GUI, gui::pauseBase);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.393, h*0.31, w*0.23, h*0.6))) {

                nk_layout_row_dynamic(GUI, h*0.12, 1);
                if (nk_button_image(GUI, gui::text_resume, gui::text_resumeHover))
                    EventManager::getInstance().addEvent(Event {EventType::Game_Pause});
                if (nk_button_image(GUI, gui::text_poptions, gui::text_poptionsHover)){
                    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
                    device->setGUIDrawFunction(drawRPS_GUI_Options);
                }
                if (nk_button_image(GUI, gui::text_menu, gui::text_menuHover)) {
                    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
                    device->setGUIDrawFunction(drawRPS_GUI_Menu);
                    EventData eData;
                    eData.Id = IGameState::stateType::INTRO;
                    EventManager::getInstance().addEvent(Event {EventType::State_Change, eData});
                }
                if (nk_button_image(GUI, gui::text_pexit, gui::text_pexitHover))
                    EventManager::getInstance().addEvent(Event {EventType::Game_Close});
                nk_popup_end(GUI);
            }
            
		}
    

	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void gui::init() {

    for(int i = 0; i < textures.size(); i++) {
        glDeleteTextures(1, &textures[i]);
    }
    textures.clear();

    //==========================================================================================
    //  MAIN MENU
    //==========================================================================================
    if(GlobalVariables::getInstance().getLanguage() == 0) {
        gui::menuBase                   =   gui::loadTexture("media/img/GUI/MainMenu/ENG/menuBase.png");
        gui::text_singleplayer          =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bSingle.png");
        gui::text_singleplayerHover     =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bSingleHover.png");
        gui::text_multiplayer           =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bMultiplayer.png");
        gui::text_multiplayerHover      =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bMultiplayerHover.png");
        gui::text_options               =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bOptions.png");
        gui::text_optionsHover          =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bOptionsHover.png");
        gui::text_exit                  =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bExit.png");
        gui::text_exitHover             =   gui::loadTexture("media/img/GUI/MainMenu/ENG/bExitHover.png");
        gui::background                 =   gui::loadTexture("media/img/background.jpg");
    } 
    else {
        gui::menuBase                   =   gui::loadTexture("media/img/GUI/MainMenu/SPA/menuBase.png");
        gui::text_singleplayer          =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bSingle.png");
        gui::text_singleplayerHover     =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bSingleHover.png");
        gui::text_multiplayer           =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bMultijugador.png");
        gui::text_multiplayerHover      =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bMultijugadorHover.png");
        gui::text_options               =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bOpciones.png");
        gui::text_optionsHover          =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bOpcionesHover.png");
        gui::text_exit                  =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bSalir.png");
        gui::text_exitHover             =   gui::loadTexture("media/img/GUI/MainMenu/SPA/bSalirHover.png");
        gui::background                 =   gui::loadTexture("media/img/background.jpg");
    }

    //==========================================================================================
    //  OPTIONS MENU
    //==========================================================================================
    if(GlobalVariables::getInstance().getLanguage() == 0) {
        gui::optionsBase                =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/opcionesBase.png");
        gui::text_language              =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bLanguageEnglish.png");
        gui::text_languageHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bLanguageEnglishHover.png");
        gui::text_soundON               =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bSoundOn.png");
        gui::text_soundONHover          =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bSoundOnHover.png");
        gui::text_soundOFF              =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bSoundOff.png");
        gui::text_soundOFFHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bSoundOffHover.png");
        gui::text_volume                =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bVolume.png");
        gui::text_volumeHover           =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bVolumeHover.png");
        gui::text_oexit                 =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bExit.png");
        gui::text_oexitHover            =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bExitHover.png");
    } 
    else {
        gui::optionsBase                =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/opcionesBase.png");
        gui::text_language              =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bIdiomaEspañol.png");
        gui::text_languageHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bIdiomaEspañolHover.png");
        gui::text_soundON               =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOn.png");
        gui::text_soundONHover          =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOnHover.png");
        gui::text_soundOFF              =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOff.png");
        gui::text_soundOFFHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOffHover.png");
        gui::text_volume                =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bVolume.png");
        gui::text_volumeHover           =   gui::loadTexture("media/img/GUI/OptionsMenu/ENG/bVolumeHover.png");
        gui::text_oexit                 =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSalir.png");
        gui::text_oexitHover            =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSalirHover.png");
    }

    //==========================================================================================
    //  PAUSE MENU
    //==========================================================================================
    if(GlobalVariables::getInstance().getLanguage() == 0) {
        gui::pbackground         =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/background.png");
        gui::pauseBase           =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/pauseBase.png");
        gui::text_resume         =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bResume.png");
        gui::text_resumeHover    =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bResumeHover.png");
        gui::text_poptions       =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bOptions.png");
        gui::text_poptionsHover  =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bOptionsHover.png");
        gui::text_menu           =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bMainMenu.png");
        gui::text_menuHover      =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bMainMenuHover.png");
        gui::text_pexit          =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bExitGame.png");
        gui::text_pexitHover     =   gui::loadTexture("media/img/GUI/PauseMenu/ENG/bExitGameHover.png");
    } 
    else {

        gui::pbackground         =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/background.png");
        gui::pauseBase           =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/pausaBase.png");
        gui::text_resume         =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bContinuar.png");
        gui::text_resumeHover    =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bContinuarHover.png");
        gui::text_poptions       =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bOpciones.png");
        gui::text_poptionsHover  =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bOpcionesHover.png");
        gui::text_menu           =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bMenuPrincipal.png");
        gui::text_menuHover      =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bMenuPrincipalHover.png");
        gui::text_pexit          =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bSalir.png");
        gui::text_pexitHover     =   gui::loadTexture("media/img/GUI/PauseMenu/SPA/bSalirHover.png");
    }

    //==========================================================================================
    //  PLAYER  INTERFACE
    //==========================================================================================
    gui::minimap        = gui::loadTexture("media/img/GUI/PlayerInterface/minimap.png");
    gui::dot_player     = gui::loadTexture("media/img/GUI/PlayerInterface/player.png");
    gui::dot_enemy      = gui::loadTexture("media/img/GUI/PlayerInterface/enemy.png");

    gui::lap_1  =   gui::loadTexture("media/img/GUI/PlayerInterface/LapNumbers/1.png");
    gui::lap_2  =   gui::loadTexture("media/img/GUI/PlayerInterface/LapNumbers/2.png");
    gui::lap_3  =   gui::loadTexture("media/img/GUI/PlayerInterface/LapNumbers/3.png");

    gui::item_void      = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoVacio.png");
    gui::item_banana    = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoTrampa.png");
    gui::item_blue      = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoBomba.png");
    gui::item_red       = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoRueda.png");
    gui::item_star      = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoBotella.png");
    gui::item_mushroom  = gui::loadTexture("media/img/GUI/PlayerInterface/ItemIcons/iconoSetas.png");

    if(GlobalVariables::getInstance().getLanguage() == 0) {
        gui::number_1       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/ENG/1.png");
        gui::number_2       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/ENG/2.png");
        gui::number_3       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/ENG/3.png");
        gui::number_4       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/ENG/4.png");
    }
    else{
        gui::number_1       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/SPA/1.png");
        gui::number_2       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/SPA/2.png");
        gui::number_3       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/SPA/3.png");
        gui::number_4       = gui::loadTexture("media/img/GUI/PlayerInterface/Positions/SPA/4.png");
    }

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

    textures.push_back(tex);
    
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
void addPause(EventData eData) {
    IGameState::stateType state = GlobalVariables::getInstance().getGameState();

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();

    if(state == IGameState::stateType::MATCH){
        device->setGUIDrawFunction(drawRPS_GUI_Pause);
        EventData eData;
        eData.Id = IGameState::stateType::PAUSE;
        MatchState::getInstance().setRatio(0.0);
        EventManager::getInstance().addEvent(Event {EventType::State_Change, eData});
    }
    else if (state == IGameState::stateType::PAUSE){
        device->setGUIDrawFunction(drawRPS_GUI_HUD);
        EventData eData;
        eData.Id = IGameState::stateType::MATCH;
        MatchState::getInstance().setRatio(1.0);
        EventManager::getInstance().addEvent(Event {EventType::State_Change, eData});
    }
}

void changeLanguage(EventData eData) {
    gui::init();
}