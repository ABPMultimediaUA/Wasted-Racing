#include "RenderIrrlicht.h"
#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/ObjectRenderComponent.h"

void RenderIrrlicht::openWindow(){

    irr::video::E_DRIVER_TYPE driverType = irr::video::E_DRIVER_TYPE::EDT_OPENGL;

    InputIrrlicht* receiver = new InputIrrlicht();

    device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(window.size.x, window.size.y),
        16, window.fullscreen, false, window.vsync, receiver);

    videoDriver = device->getVideoDriver();
    sceneManager = device->getSceneManager();

    addCamera();
    addLight();

    uintptr_t aux = reinterpret_cast<uintptr_t>(device);
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

}

void RenderIrrlicht::updateWindow() {
    updateCamera();
}

void RenderIrrlicht::closeWindow() {

    device->drop();

}

void RenderIrrlicht::renderDraw() {

    videoDriver->beginScene(true, true, irr::video::SColor(255,113,113,133));
    sceneManager->drawAll();
    videoDriver->endScene();
 
}

void RenderIrrlicht::addCamera() {
    camera = sceneManager->addCameraSceneNode();
    camera->setPosition(irr::core::vector3df(0,0,0));
}

void RenderIrrlicht::updateCamera() {
    auto pos = cameraTarget->getTransformData().position;

    camera->setTarget(irr::core::vector3df(pos.x, pos.y, pos.z));
    //RESTORE VALUES IMPORTAAAAAAAAANT camera->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z - 30));
    camera->setPosition(irr::core::vector3df(0, 50, -50));
}

void RenderIrrlicht::addLight() {
    sceneManager->addLightSceneNode();
}

void RenderIrrlicht::addObject(IComponent::Pointer ptr) {

    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr.get());

    if(cmp != nullptr){

        auto shape = cmp->getObjectShape();
        auto obj = cmp->getGameObject();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;
        irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
        irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
        irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

        irr::scene::ISceneNode * node;

        //Initialize the node
        switch(shape){

            case ObjectRenderComponent::Shape::Cube: {
                node = sceneManager->addCubeSceneNode();
            }
            break;
            case ObjectRenderComponent::Shape::Sphere: {
                node = sceneManager->addSphereSceneNode();
            }
            break;
            default: break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);

        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::updateObjectTransform(uint16_t id, GameObject::TransformationData transform) {
    auto pos = transform.position;
    auto rot = transform.rotation;
    auto sca = transform.scale;
    irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
    irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
    irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

    auto iterator = nodeMap.find(id);
    if(iterator != nodeMap.end()){
        auto node = iterator->second;
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);
    }
}