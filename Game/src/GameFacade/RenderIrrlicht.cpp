#include "RenderIrrlicht.h"
#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/ObjectManager.h"
#include "../GameObject/ObjectRenderComponent.h"
#include <cmath>
#include <string>

void RenderIrrlicht::openWindow(){

    irr::video::E_DRIVER_TYPE driverType = irr::video::E_DRIVER_TYPE::EDT_OPENGL;

    InputIrrlicht* receiver = new InputIrrlicht();

    device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(window.size.x, window.size.y),
        16, window.fullscreen, false, window.vsync, receiver);

    videoDriver = device->getVideoDriver();
    sceneManager = device->getSceneManager();
    geometryCreator = sceneManager->getGeometryCreator();

    font = sceneManager->getGUIEnvironment()->getFont("../media/img/fontcourier.bmp");
    pos = sceneManager->getGUIEnvironment()->addStaticText(L"Position: ", irr::core::recti(0,0, 200,50));
    pos->setOverrideFont(font);

    addCamera();
    addLight();

    uintptr_t aux = reinterpret_cast<uintptr_t>(device);
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

}

void RenderIrrlicht::updateWindow() {
    updateCamera();
    int h = pos->getTextHeight();
    int w = pos->getTextWidth();
    int oM = ObjectManager::getInstance().getObject(50).get()->getComponent<ScoreComponent>().get()->getPosition();
    std::cout <<  "Height: " << h << std::endl;
    std::cout << "Width: " << w << std::endl;
    irr::core::stringw stringPos = L"  POSITION:";
    stringPos += oM;
    pos->setText(stringPos.c_str());
}

void RenderIrrlicht::closeWindow() {

    device->drop();

}

void RenderIrrlicht::renderDraw() {

    videoDriver->beginScene(true, true, irr::video::SColor(255,113,113,133));
    sceneManager->drawAll();
    sceneManager->getGUIEnvironment()->drawAll();
    videoDriver->endScene();
 
}

void RenderIrrlicht::addCamera() {
    camera = sceneManager->addCameraSceneNode();
    camera->setPosition(irr::core::vector3df(0,0,0));
}

void RenderIrrlicht::updateCamera() {
    //Get target position
    auto pos = cameraTarget->getTransformData().position;

    //Get target y angle
    float radianAngle = cameraTarget->getTransformData().rotation.y;

    camera->setTarget(irr::core::vector3df(pos.x, pos.y, pos.z));
    camera->setPosition(irr::core::vector3df(pos.x - 30*cos(radianAngle), pos.y + 5, pos.z + 30*sin(radianAngle)));
}

void RenderIrrlicht::addLight() {
    auto node = sceneManager->addLightSceneNode(); 
    node->setPosition(irr::core::vector3df(0,150,0));
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
            case ObjectRenderComponent::Shape::Plane: {
                auto plane = geometryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1,1));
                node = sceneManager->addMeshSceneNode(plane);
            }
            default: break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);

        auto var = videoDriver->getTexture("media/img/stones.jpg");
        node->setMaterialTexture(0, var);
        std::cout << var << std::endl;
        //while(true);

        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::deleteObject(IComponent::Pointer ptr) {
    
    auto id = ptr.get()->getGameObject().getId();
    auto itr = nodeMap.find(id);

    if(itr != nodeMap.end()){
        auto node = itr->second;
        node->remove();
        nodeMap.erase(id);
    }
}

void RenderIrrlicht::updateObjectTransform(uint16_t id, GameObject::TransformationData transform) {
    auto pos = transform.position;
    auto rot = transform.rotation;
    auto sca = transform.scale;
    irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
    irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

    auto iterator = nodeMap.find(id);
    if(iterator != nodeMap.end()){

        irr::core::quaternion quadX = irr::core::vector3df(rot.x,0,0);
        irr::core::quaternion quadY = irr::core::vector3df(0,rot.y,0);
        irr::core::quaternion quadZ = irr::core::vector3df(0,0,rot.z);

        irr::core::quaternion quadRes = quadY*quadX*quadZ;

        irr::core::vector3df vec;
        quadRes.toEuler(vec);

        irr::core::vector3df vecRes;

        vecRes.X = vec.X*180/M_PI;
        vecRes.Y = vec.Y*180/M_PI;
        vecRes.Z = vec.Z*180/M_PI;

        auto node = iterator->second;
        node->setPosition(irrPos);
        node->setRotation(vecRes);
        node->setScale(irrSca);
    }
}