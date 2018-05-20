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
#include "../GameObject/RenderComponent/AnimationRenderComponent.h"
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
void drawRPS_GUI_CityName(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_Countdown(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_AfterMatch(); //:::> function that is given as parameter to redpanda
void drawRPS_GUI_LoadingScreen(); //:::> function that is given as parameter to redpanda

namespace gui {

    //For controlling animations
    const int bkgSize = 6;
    int currImg = 0;
    int currFrame = 0;

    #ifdef __WIN32__
        const int frameSkip = 10;
    #else
        const int frameSkip = 3;
    #endif

    //MAIN  MENU Images
    struct nk_image background[bkgSize];
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

    //SELECTION Images
    struct nk_image post_left;
    struct nk_image post_leftHover;
    struct nk_image post_right;
    struct nk_image post_rightHover;
    struct nk_image post_up;
    struct nk_image text_punk;
    struct nk_image text_croco;
    struct nk_image text_cyborg;
    struct nk_image text_witch;
    struct nk_image text_punkHover;
    struct nk_image text_crocoHover;
    struct nk_image text_cyborgHover;
    struct nk_image text_witchHover;

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
    struct nk_image text_currentSound;
    struct nk_image text_currentSoundHover;

    //OTHER Images
    struct nk_image loadingScreen;
    struct nk_image loadedScreen;
    struct nk_image cityName;
    struct nk_image countdown[4];
    struct nk_image winner;
    struct nk_image looser;

    void init();
    struct nk_image loadTexture(const char* path);
    void playAnimation();

    std::vector<GLuint> textures;
    static size_t volume = 100;   

}

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addMenu(EventData eData);
void addHUD(EventData eData);
void addCityName(EventData eData);
void addCountdown(EventData eData);
void addPause(EventData eData);
void addResult(EventData eData);
void addLoadingScreen(EventData eData);
void addSelection(EventData eData);
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

    EventManager::getInstance().addListener(EventListener {EventType::Game_Menu, addMenu});
    EventManager::getInstance().addListener(EventListener {EventType::Game_PlayerSelection, addSelection});
    EventManager::getInstance().addListener(EventListener {EventType::Game_LoadingScreen, addLoadingScreen});
    EventManager::getInstance().addListener(EventListener {EventType::Match_Start, addCityName});
    EventManager::getInstance().addListener(EventListener {EventType::Match_Countdown, addCountdown});
    EventManager::getInstance().addListener(EventListener {EventType::Match_Race_Start, addHUD});
    EventManager::getInstance().addListener(EventListener {EventType::Match_Race_Start, addHUD});
    EventManager::getInstance().addListener(EventListener {EventType::Match_Race_End, addResult});
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
    valueY = 0.3;
    sum = 0;
}

//Update the current camera
void RenderRedPanda::interpolateCamera(float accTime, float maxTime) { 

    //Get target position
    auto pos = cameraTarget->getTransformData().position;

    //Get target y angle
    float radianAngle = cameraTarget->getTransformData().rotation.y;

    auto camera = cameraTarget->getComponent<CameraRenderComponent>().get();

    //Get interpolated distance to the player
    float oldD = camera->getOldDistance();
    float newD = camera->getDistance();
    
    float distance = oldD + (accTime * (newD - oldD))/maxTime;
    distance *= 1.3;

    auto player = camera->getGameObject();
    auto vel = player.getComponent<MoveComponent>()->getMovemententData().vel;
    auto maxVel = player.getComponent<MoveComponent>()->getMovemententData().max_vel;
    auto oldPosPlayer = player.getOldTransformData().position;
    auto newPosPlayer = player.getNewTransformData().position;

    auto mData = player.getComponent<MoveComponent>()->getMovemententData();

    auto posPlayer = oldPosPlayer.y - newPosPlayer.y;
    if(posPlayer > 0.25 && posPlayer < 2 && sum < 20)
    {
        sum += 0.25;
    }
    else if(posPlayer < -0.25 && posPlayer > -2 && sum > -20)
    {
        sum -= 0.25;
    }
    else if(posPlayer < 0.25 && posPlayer > -0.25 && vel > maxVel/10)
    {
        if(sum > 0)
        {
            sum -= 0.25;
        }
        else if(sum < 0)
        {
            sum += 0.25; 
        }
    }

    if(newD > 15)
    {
        if(valueY > 0.4)
        {
            valueY -= 0.02;
        }
        if(!(mData.jump == false && mData.asc == false))
        {
            if(sum > 0)
            {
                sum -= 0.25;
            }
            else if(sum < 0)
            {
                sum += 0.25; 
            }
        }
    }
    else
    {
        if(valueY < 1.2)
        {
            valueY += 0.02;
        }
        if(!(mData.jump == false && mData.asc == false))
        {
            if(sum > 0)
            {
                sum -= 0.25;
            }
            else if(sum < 0)
            {
                sum += 0.25; 
            }
        }
    }

    //Define position and target
    glm::vec3 target(-pos.x, pos.y+15, pos.z);
    glm::vec3 position(-pos.x + distance * sin(radianAngle + glm::half_pi<float>()), pos.y+sum + distance * valueY, pos.z - distance * cos(radianAngle + glm::half_pi<float>()));
    position = position;
    
    //Update camera
    device->updateCamera(position, target);
}

void RenderRedPanda::setCameraTarget(glm::vec3 position, glm::vec3 target) {
    device->updateCamera(position, target);
}

//Add an object to the game
void RenderRedPanda::addObject(IComponent* ptr) { 
    
    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr);

    auto shape = cmp->getObjectShape();
    auto obj = cmp->getGameObject();
    auto pos = obj.getTransformData().position;
    auto sca = obj.getTransformData().scale;
    auto rot = obj.getTransformData().rotation;

    TNode * node = nullptr;
    //Initialize the node
    switch(shape){
        case ObjectRenderComponent::Shape::Mesh: {
            node = device->createObjectNode(device->getSceneRoot(), glm::vec3(0,0,0), cmp->getMesh().c_str());
            rps::scaleNode(node,sca);
        }
        break;
        case ObjectRenderComponent::Shape::Cube: {
            node = device->createObjectNode(device->getSceneRoot(), glm::vec3(0,0,0), "media/mesh/box2/box2.obj");
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
    rps::rotateNode(node, rot);
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

//Add an animation to the game
void RenderRedPanda::addAnimation(IComponent* ptr) {

    AnimationRenderComponent* cmp = dynamic_cast<AnimationRenderComponent*>(ptr);

    auto obj = cmp->getGameObject();
    auto pos = obj.getTransformData().position;
    auto sca = obj.getTransformData().scale;
    auto rot = obj.getTransformData().rotation;
    
    TNode * node = device->createAnimatedNode(device->getSceneRoot(), glm::vec3(0,0,0), cmp->getPath().c_str(), true, cmp->getFrames(), 1/24.0);

    rps::translateNode(node, glm::vec3(-pos.x, pos.y, pos.z));
    rps::scaleNode(node, sca);
    rps::rotateNode(node, rot);

    ((TAnimation*)node->getEntity())->setRender(true);

    nodeMap.insert(std::pair<uint16_t, TNode*>(obj.getId(), node));
    animationMap.insert(std::pair<uint16_t, TAnimation*>(obj.getId(), (TAnimation*)node->getEntity()));

}

//Add an animation to the game
void RenderRedPanda::addAnimation(uint16_t id, const char * mesh, int frames, const char* texture) 
{
    std::string s = std::string("media/anim/") + std::string(mesh);

    std::string t = std::string("media/anim/") + std::string(texture);

    TNode * node = device->createAnimatedNode(device->getSceneRoot(), glm::vec3(0,0,0), s.c_str(), true, frames, 1/24.0, t.c_str());

    animationMap.insert(std::pair<uint16_t, TAnimation*>(id, (TAnimation*)node->getEntity()));

}

void RenderRedPanda::deleteAnimation(const char * animation)
{
    std::cout << "Delete Animation: " << animation << std::endl;
    std::string s = std::string("media/anim/") + std::string(animation);

    device->deleteAnimation(s.c_str());
}

void RenderRedPanda::stopAnimation(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr) {
        t->setPause(true);
    }

}
void RenderRedPanda::loopOnceAnimation(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr) {
        t->reset();
        t->setPause(true);
        t->setLoop(false);
    }

}
void RenderRedPanda::playAnimation(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr) {
        t->reset();
        t->setPause(true);
    }

}
void RenderRedPanda::loopAnimation(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr) {
        t->reset();
        t->setPause(true);
        t->setLoop(true);
    }

}
void RenderRedPanda::resetAnimation(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr) {
        t->reset();
    }

}
void RenderRedPanda::changeAnimation(uint16_t id, uint16_t animation) {

    TAnimation* character = animationMap[id];

    auto a = (ObjectManager::getInstance().getObject(id)).get()->getComponent<AnimationRenderComponent>();

    if(a != nullptr) {
        animation += 10 * a.get()->getPlayer();
    }
    
    TAnimation* anim = animationMap[animation];

    if(character != nullptr && anim != nullptr) {

        character->setAnimation(anim->getAnimation());

        character->setFrames(anim->getFrames());

        character->setPause(true);
        character->setLoop(true);
        character->reset();
    }

}
bool RenderRedPanda::isAnimationPLaying(uint16_t id) {

    TAnimation* t = animationMap[id];

    if(t != nullptr)
        return t->getPauseState();

    return false;

}

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
        rps::translateNode(node, glm::vec3(-pos.x, pos.y, pos.z));
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
        animationMap.erase(id);
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

        if(id >= 25000 && id <= 25010){
            rps::rotateNode(node, glm::vec3(rot.x, -rot.y-glm::half_pi<float>(), rot.z));
        }
        else {
            rps::rotateNode(node, glm::vec3(rot.x, -rot.y, rot.z));
        }
        
        if(id == 25000)
            rps::scaleNode(node, glm::vec3(0.7,0.7,0.7));
        else if (id > 25000 && id < 25010)
            rps::scaleNode(node, glm::vec3(2,2,2));
        else 
            rps::scaleNode(node, sca);

        rps::translateNode(node, glm::vec3(-pos.x, pos.y, pos.z));   
    }
}

//Update game animations
void RenderRedPanda::updateAnimations(float dTime) {
    
    if(animationMap.find(25000) != animationMap.end()) {
        animationMap[25000]->update(dTime);
        animationMap[25001]->update(dTime);
        animationMap[25002]->update(dTime);
        animationMap[25003]->update(dTime);
    }
    else if(animationMap.find(60001) != animationMap.end()) {
        animationMap[60001]->update(dTime);
        animationMap[60002]->update(dTime);
        animationMap[60003]->update(dTime);
        animationMap[60004]->update(dTime);
    }
    
}

//Update single animation
void RenderRedPanda::updateAnimation(IComponent* ptr) {

    AnimationRenderComponent* anim = (AnimationRenderComponent*)ptr;

    int state = anim->getState();
    int16_t id = anim->getGameObject().getId();

    TAnimation* node = animationMap.find(id)->second;

    if(state == 0) {
        node->setPause(true);
    }
    else if(state == 1) {
        node->playNoLoop();
        node->setLoop(false);
    }
    else if(state == 2) {
        node->playNoLoop();
        node->setLoop(true);
    }
}

//add mesh lod
void RenderRedPanda::addMeshLoD(int lvl, const char* mesh)
{
    if(lvl > 0 && std::strcmp(mesh, "") != 0)
    {
        device->addMeshLoD(lvl, mesh);
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
                gui::playAnimation();
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

void drawRPS_GUI_CityName() {
    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                nk_image(GUI, gui::cityName);
                nk_popup_end(GUI);
            }
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_Countdown() {
    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                nk_image(GUI, gui::countdown[GlobalVariables::getInstance().getCountdown()]);
                nk_popup_end(GUI);
            }
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_AfterMatch() {
    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);
                if((RenderManager::getInstance().getRenderFacade()->getCameraTarget()).getComponent<ScoreComponent>().get()->getPosition() == 1) {
                    nk_image(GUI, gui::winner);
                }
                else {
                    nk_image(GUI, gui::looser);
                }
                nk_popup_end(GUI);
            }
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_LoadingScreen() {
    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {

            GUI->style.window.fixed_background = nk_style_item_hide();
            
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -5, w+15, h+6))) {
                nk_layout_row_static(GUI, h, w, 1);

                if(GlobalVariables::getInstance().getGameLoaded())
                    nk_image(GUI, gui::loadedScreen);
                else
                    nk_image(GUI, gui::loadingScreen);

                nk_popup_end(GUI);
            }
		}
	nk_end(GUI);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void drawRPS_GUI_PlayerSelect() {

    Window window = RenderManager::getInstance().getRenderFacade()->getWindow();
    int w = window.size.x;
    int h = window.size.y;

    int currPlayer = GlobalVariables::getInstance().getSelectedPlayer();
    
    if (nk_begin(GUI, "Demo", nk_rect(0, 0, window.size.x, window.size.y),0))
        {
            //Upper bar
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(-13, -h*0.05, w+15, h+6))) {
                nk_layout_row_static(GUI, w*0.194, w, 1);
                nk_image(GUI, gui::post_up);
                nk_popup_end(GUI);
            }
            //Left sign
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.06, h*0.254, w*0.3, h*0.8))) {

                nk_layout_row_static(GUI, w*0.42, w*0.168, 1);
                if (nk_button_image(GUI, gui::post_left, gui::post_leftHover)){
                    if(!GlobalVariables::getInstance().getSelecting()) {
                        if(currPlayer > 0)
                            GlobalVariables::getInstance().setSelectedPlayer(currPlayer-1);
                        else
                            GlobalVariables::getInstance().setSelectedPlayer(3);
                    }
                }
                nk_popup_end(GUI);
            }
            //Right sign
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.7, h*0.254, w*0.3, h*0.8))) {

                nk_layout_row_static(GUI, w*0.42, w*0.168, 1);
                if (nk_button_image(GUI, gui::post_right, gui::post_rightHover)){
                    if(!GlobalVariables::getInstance().getSelecting()) {
                        if(currPlayer < 3)
                            GlobalVariables::getInstance().setSelectedPlayer(currPlayer+1);
                        else
                            GlobalVariables::getInstance().setSelectedPlayer(0);
                    }
                    
                }
                nk_popup_end(GUI);
            }
            //Lower sign
            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(w*0.35, h*0.7, w*0.33, w*0.2))) {

                nk_layout_row_static(GUI, w*0.142, w*0.3, 1);
                
                if(currPlayer == 0) {
                    if (nk_button_image(GUI, gui::text_punk, gui::text_punkHover))
                        GlobalVariables::getInstance().setFixedPlayer(true);
                }
                else if(currPlayer == 1) {
                    if (nk_button_image(GUI, gui::text_croco, gui::text_crocoHover))
                        GlobalVariables::getInstance().setFixedPlayer(true);
                }
                else if(currPlayer == 2) {
                    if (nk_button_image(GUI, gui::text_cyborg, gui::text_cyborgHover))
                        GlobalVariables::getInstance().setFixedPlayer(true);
                }
                else if(currPlayer == 3) {
                    if (nk_button_image(GUI, gui::text_witch, gui::text_witchHover))
                        GlobalVariables::getInstance().setFixedPlayer(true);
                }
                
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
                    gui::playAnimation();
                else if(GlobalVariables::getInstance().getGameState() == IGameState::stateType::PAUSE)
                    nk_image(GUI, gui::pbackground);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(h*0.1388, 0, w, h))) {
                nk_layout_row_static(GUI, h, h*1.5, 1);
                nk_image(GUI, gui::optionsBase);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.16, h*0.235, w*0.21, w*0.06))) {
                nk_layout_row_static(GUI, w*0.0568, w*0.2, 1);

                float vol = GlobalVariables::getInstance().getVolume();

                if(vol > 0){
                    gui::text_currentSound      = gui::text_soundON;
                    gui::text_currentSoundHover = gui::text_soundONHover;
                }
                else {
                    gui::text_currentSound      = gui::text_soundOFF;
                    gui::text_currentSoundHover = gui::text_soundOFFHover;
                }

                if (nk_button_image(GUI, gui::text_currentSound, gui::text_currentSoundHover)){
                    
                    if(vol == 0)       vol = (float)gui::volume/100.0;
                    else if (vol > 0)  vol = 0;

                    EventManager::getInstance().addEvent(Event {EventType::Global_ChangeVolume});

                    GlobalVariables::getInstance().setVolume(vol);

                }           
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(h*0.2, h*0.415, w*0.2, w*0.06))) {
                nk_layout_row_static(GUI, w*0.043, w*0.15, 1);
                nk_image(GUI, gui::text_volume);
                nk_popup_end(GUI);
            }

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.26, h*0.42, w*0.2, w*0.06))) {  

                nk_layout_row_static(GUI, w*0.025, w*0.14, 1);

                if(nk_progress(GUI, &gui::volume, 100, nk_true)) {

                    GlobalVariables::getInstance().setVolume((float)gui::volume/100.0);

                }

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
                case 1: 
                        nk_image(GUI, gui::number_1);
                        break;
                case 2: 
                        nk_image(GUI, gui::number_2);
                        break;
                case 3: 
                        nk_image(GUI, gui::number_3);
                        break;
                case 4: 
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

            if (nk_popup_begin(GUI, NK_POPUP_STATIC, "Image Popup", NK_WINDOW_NO_SCROLLBAR, nk_rect(w*0.89 - pos1.z * h * 0.00018, h*0.65 - pos1.x * h * 0.00018 * 0.5, w*0.2, h*0.5))) {

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

    for(unsigned int i = 0; i < textures.size(); i++) {
        glDeleteTextures(1, &textures[i]);
    }
    textures.clear();

    //==========================================================================================
    //  BACKGROUND & OTHERS
    //==========================================================================================
    gui::background[0]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck1.gif");
    gui::background[1]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck2.gif");
    gui::background[2]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck3.gif");
    gui::background[3]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck4.gif");
    gui::background[4]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck5.gif");
    gui::background[5]                  =   gui::loadTexture("media/img/GUI/Background/New/Bck6.gif");
    gui::cityName                       =   gui::loadTexture("media/img/GUI/Other/cityName.png");
    gui::countdown[1]                   =   gui::loadTexture("media/img/GUI/Other/1.png");
    gui::countdown[2]                   =   gui::loadTexture("media/img/GUI/Other/2.png");
    gui::countdown[3]                   =   gui::loadTexture("media/img/GUI/Other/3.png");

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
        gui::loadingScreen              =   gui::loadTexture("media/img/GUI/Other/loadScreenENG.png");
        gui::loadedScreen               =   gui::loadTexture("media/img/GUI/Other/loadedScreenENG.png");
        gui::countdown[0]               =   gui::loadTexture("media/img/GUI/Other/startENG.png");
        gui::winner                     =   gui::loadTexture("media/img/GUI/Other/winnerENG.png");
        gui::looser                     =   gui::loadTexture("media/img/GUI/Other/looserENG.png");
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
        gui::loadingScreen              =   gui::loadTexture("media/img/GUI/Other/loadScreenSPA.png");
        gui::loadedScreen               =   gui::loadTexture("media/img/GUI/Other/loadedScreenSPA.png");
        gui::countdown[0]               =   gui::loadTexture("media/img/GUI/Other/startSPA.png");
        gui::winner                     =   gui::loadTexture("media/img/GUI/Other/winnerSPA.png");
        gui::looser                     =   gui::loadTexture("media/img/GUI/Other/looserSPA.png");
    }

    //==========================================================================================
    //  PLAYER SELECTION
    //==========================================================================================
    if(GlobalVariables::getInstance().getLanguage() == 0) {
        gui::post_left                  =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/bIzq.png");
        gui::post_leftHover             =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/bIzqHover.png");
        gui::post_right                 =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/bDrch.png");
        gui::post_rightHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/bDrchHover.png");
        gui::post_up                    =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/superiorBase.png");
        gui::text_punk                  =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/NeonDemon.png");
        gui::text_croco                 =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/KillahDryla.png");
        gui::text_cyborg                =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/GameEnder.png");
        gui::text_witch                 =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/UltraViolet.png");
        gui::text_punkHover             =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/NeonDemonHover.png");
        gui::text_crocoHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/KillahDrylaHover.png");
        gui::text_cyborgHover           =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/GameEnderHover.png");
        gui::text_witchHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/ENG/UltraVioletHover.png");
    } 
    else {
        gui::post_left                  =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/bIzq.png");
        gui::post_leftHover             =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/bIzqHover.png");
        gui::post_right                 =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/bDrch.png");
        gui::post_rightHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/bDrchHover.png");
        gui::post_up                    =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/superiorBase.png");
        gui::text_punk                  =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/NeonDemon.png");
        gui::text_croco                 =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/KillahDryla.png");
        gui::text_cyborg                =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/GameEnder.png");
        gui::text_witch                 =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/UltraViolet.png");
        gui::text_punkHover             =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/NeonDemonHover.png");
        gui::text_crocoHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/KillahDrylaHover.png");
        gui::text_cyborgHover           =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/GameEnderHover.png");
        gui::text_witchHover            =   gui::loadTexture("media/img/GUI/CharacterSelect/SPA/UltraVioletHover.png");
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
        gui::text_language              =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bIdiomaEspanol.png");
        gui::text_languageHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bIdiomaEspanolHover.png");
        gui::text_soundON               =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOn.png");
        gui::text_soundONHover          =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOnHover.png");
        gui::text_soundOFF              =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOff.png");
        gui::text_soundOFFHover         =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bSonidoOffHover.png");
        gui::text_volume                =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bVolumen.png");
        gui::text_volumeHover           =   gui::loadTexture("media/img/GUI/OptionsMenu/SPA/bVolumenHover.png");
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

//Play animation loop
void gui::playAnimation() {

    nk_image(GUI, gui::background[currImg]); 

    if(currFrame == frameSkip) {

        if(currImg == bkgSize - 1)
            currImg = 0;
        else 
            currImg++;

        currFrame = 0;
    }
    else {
        currFrame++;
    }
}

//==============================================================
// VISUAL EFFECTS
//==============================================================
//Set the postprocessing state
void RenderRedPanda::setPostProcessing(bool b)
{
    device->setPPActive(b);
}

//Set the current postprocessing option to render
void RenderRedPanda::setPostProcessingOption(int o)
{
    device->setPPOption((rps::PPOption) o);
}

//Set the blur effect
void RenderRedPanda::setBlurEffect(bool b)
{
    if(b)
    {
        //Check postprocessing
        if(!device->getPPActive())
            setPostProcessing(true);

        //Set blur
        device->setPPOption(rps::PPOption::BLUR);
    }
    else
    {
        //Check postprocessing
        if(device->getPPActive())
            setPostProcessing(false);

        //Set blur
        device->setPPOption(rps::PPOption::DEFAULT);
    }
}

//Set the blur effect origin
void RenderRedPanda::setBlurOrigin(float x, float y)
{
    device->setPPBlurPos(x, y);
}

//Set the blur effect effect intensity
void RenderRedPanda::setBlurIntensity(float i)
{
    device->setPPBlurStrength(i);
}

//Set the blur effect radius
void RenderRedPanda::setBlurRadius(float r)
{
    device->setPPBlurDist(r);
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

void addCityName(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_CityName);

}

void addCountdown(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_Countdown);

}

void addResult(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_AfterMatch);

}

void addLoadingScreen(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_LoadingScreen);

}

void addMenu(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_Menu);

}

void addSelection(EventData eData) {

    rps::RedPandaStudio *device = dynamic_cast<RenderRedPanda*>(RenderManager::getInstance().getRenderFacade())->getDevice();
    device->setGUIDrawFunction(drawRPS_GUI_PlayerSelect);

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
    else if (state == IGameState::stateType::PREMATCH) {
        EventData eData;
        eData.Id = IGameState::stateType::MATCH;
        EventManager::getInstance().addEvent(Event {EventType::State_Change, eData});
        GlobalVariables::getInstance().setIgnoreInput(false);
    }

}

void changeLanguage(EventData eData) {
    gui::init();
}