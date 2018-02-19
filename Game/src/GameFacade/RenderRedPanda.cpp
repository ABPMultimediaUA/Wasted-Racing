#include "RenderRedPanda.h"

//==============================================================
// Engine Related functions
//==============================================================
//Creates a window depending on the engine
void RenderRedPanda::openWindow() { 

    device = &rps::RedPandaStudio::createDevice(window.size.x,window.size.y,32,0,window.vsync,window.fullscreen);

    InputRedPanda* receiver = new InputRedPanda();

    uintptr_t aux = reinterpret_cast<uintptr_t>(device->getWindow());
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

}

//Updates window info in the engine
void RenderRedPanda::updateWindow() { 

}

//Closes engine window
void RenderRedPanda::closeWindow() { 

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

    if(cmp != nullptr){

        auto shape = cmp->getObjectShape();
        auto obj = cmp->getGameObject();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;

        TNode * node;

        //Initialize the node
        switch(shape){

            case ObjectRenderComponent::Shape::Mesh: {
                node = device->createObjectNode(device->getSceneRoot(), pos, cmp->getMesh().c_str());
                std::cout << cmp->getMesh() << std::endl;
            }
            break;
            default:
            break;
        }
    
        nodeMap.insert(std::pair<uint16_t, TNode*>(obj.getId(), node));
    }

}

//Add an object to the game (Cylinder or Cone)
void RenderRedPanda::addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency) { }

//Add a light to the game
void RenderRedPanda::addLight(IComponent* ptr) { }

//Delete an object of the game
void RenderRedPanda::deleteObject(IComponent* ptr) { }

//Change the position of an object in-game
void RenderRedPanda::updateObjectTransform(uint16_t id, GameObject::TransformationData transform) { }

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

void RenderRedPanda::drawGUI() { }

void RenderRedPanda::createItemIcon(glm::vec2 pos, std::string img) { }

void RenderRedPanda::deleteItemIcon() { }

void RenderRedPanda::updateItemIcon() { }