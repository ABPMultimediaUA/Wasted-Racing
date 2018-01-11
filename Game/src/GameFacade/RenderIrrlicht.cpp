#include "RenderIrrlicht.h"
#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/ObjectManager.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
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
    lap = sceneManager->getGUIEnvironment()->addStaticText(L"Lap: ", irr::core::recti(0, 20, 200, 50));
    item = sceneManager->getGUIEnvironment()->addStaticText(L"Item: ", irr::core::recti(0, 40, 200, 50));
    pos->setOverrideFont(font);

    addCamera();
    addLight();

    uintptr_t aux = reinterpret_cast<uintptr_t>(device);
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

}

void RenderIrrlicht::updateWindow() {
    updateCamera();
    int oM = ObjectManager::getInstance().getObject(50).get()->getComponent<ScoreComponent>().get()->getPosition();
    int oL = ObjectManager::getInstance().getObject(50).get()->getComponent<ScoreComponent>().get()->getLap();
    int iT = ObjectManager::getInstance().getObject(50).get()->getComponent<ItemHolderComponent>().get()->getItemType();
    irr::core::stringw stringLap = L"  LAP:";
    irr::core::stringw stringItm = L"  ITEM:";
    irr::core::stringw stringPos = L"  POSITION:";
    stringLap += oL;
    switch(iT)
    {
        case -1: stringItm+="EMPTY";
                 break;
        case 0: stringItm+="RED SHELL";
                 break;
        case 1: stringItm+="BLUE SHELL";
                 break;
        case 2: stringItm+="BANANA";
                 break;
        case 3: stringItm+="MUSHROOM";
                 break;
        case 4: stringItm+="STAR";
                 break;
    }
    stringPos += oM;
    pos->setText(stringPos.c_str());
    lap->setText(stringLap.c_str());
    item->setText(stringItm.c_str());
}

void RenderIrrlicht::closeWindow() {

    device->drop();

}

void RenderIrrlicht::renderDraw() {

    videoDriver->beginScene(true, true, irr::video::SColor(255,10,10,40));
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
    camera->setPosition(irr::core::vector3df(pos.x - 30*cos(radianAngle), pos.y + 12, pos.z + 30*sin(radianAngle)));
}

void RenderIrrlicht::addLight() {
    auto pLight = sceneManager->addLightSceneNode(0, irr::core::vector3df(0,0,0), irr::video::SColorf(1.0,1.0,1.0), 500); 
    auto & l = pLight->getLightData();
    l.Type = irr::video::E_LIGHT_TYPE::ELT_DIRECTIONAL;
    auto node = sceneManager->addLightSceneNode(0, irr::core::vector3df(0,0,0), irr::video::SColorf(1.0,1.0,1.0), 500); 
    node->setPosition(irr::core::vector3df(0,150,0));
    sceneManager->setAmbientLight(irr::video::SColorf(0.2,0.2,0.2,1));
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
                if(cmp->getGameObject().getId() == 18) {
                    auto var = videoDriver->getTexture("media/img/grass.jpg");
                    node->setMaterialTexture(0, var);
                }
                else if(cmp->getGameObject().getId() >= 25 && cmp->getGameObject().getId() <= 36) {
                    auto var = videoDriver->getTexture("media/img/itemBox.png");
                    node->setMaterialTexture(0, var);
                }
                else {
                    auto var = videoDriver->getTexture("media/img/road.jpg");
                    node->setMaterialTexture(0, var);
                }
            }
            break;
            case ObjectRenderComponent::Shape::Sphere: {
                node = sceneManager->addSphereSceneNode();
                auto var = videoDriver->getTexture("media/img/pool.jpg");
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Plane: {
                auto plane = geometryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1,1));
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture("media/img/ramp.jpg");
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Banana: {
                auto plane = sceneManager->getMesh("media/mesh/banana.3ds");
                node = sceneManager->addMeshSceneNode(plane);
            }
            break;
            case ObjectRenderComponent::Shape::Shell: {
                auto plane = sceneManager->getMesh("media/mesh/ball.3ds");
                node = sceneManager->addMeshSceneNode(plane);
            }
            break;
            case ObjectRenderComponent::Shape::Mesh: {
                /*
                auto plane = sceneManager->getMesh("media/mesh/amyrose.3ds");
                node = sceneManager->addMeshSceneNode(plane);*/
                auto plane = sceneManager->getMesh("media/mesh/Link.obj");
                node = sceneManager->addMeshSceneNode(plane);
            }
            break;
            case ObjectRenderComponent::Shape::Road: {
                //auto plane = sceneManager->getMesh("media/mesh/circuit.3ds");
                auto plane = sceneManager->getMesh("media/mesh/circuit.obj");
                node = sceneManager->addMeshSceneNode(plane);
            }
            break;
            case ObjectRenderComponent::Shape::StarLine: {
                auto plane = geometryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1,1));
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture("media/img/starLine.png");
                node->setMaterialTexture(0, var);
            }
            break;
            default:
            break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);

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