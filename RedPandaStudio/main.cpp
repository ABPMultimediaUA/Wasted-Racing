#include <RedPanda.h>
#include <iostream>

int main() {

    EngineFacade ef;

    ef.setWidth(1280);
    ef.setHeight(720);
    ef.setName("XKating");

    ef.init();

    //---- Crear la estructura del árbol ----
    TNode *Escena = new TNode();
    TNode *RotaLuz = new TNode(Escena);
    TNode *RotaCam = new TNode(Escena);
    TNode *RotaCoche = new TNode(Escena);
    Escena->addChild (RotaLuz);
    Escena->addChild (RotaCam);
    Escena->addChild (RotaCoche);
    TNode *TraslaLuz = new TNode(RotaLuz);
    TNode *TraslaCam = new TNode(RotaCam);
    TNode *TraslaCoche = new TNode(RotaCoche);
    RotaLuz->addChild (TraslaLuz);
    RotaCam->addChild (TraslaCam);
    RotaCoche->addChild (TraslaCoche);
    
    //---- Añadir las entidades a los nodos ----
    TTransform *TransfRotaLuz = new TTransform ();
    TTransform *TransfRotaCam = new TTransform ();
    TTransform *TransfRotaCoche = new TTransform ();
    RotaLuz->setEntity(TransfRotaLuz);
    RotaCam->setEntity(TransfRotaCam);
    RotaCoche->setEntity(TransfRotaCoche);

    TTransform *TransfTraslaLuz = new TTransform ();
    TTransform *TransfTraslaCam = new TTransform ();
    TTransform *TransfTraslaCoche = new TTransform ();
    TraslaLuz->setEntity(TransfTraslaLuz);
    TraslaCam->setEntity(TransfTraslaCam);
    TraslaCoche->setEntity(TransfTraslaCoche);

    TNode *NLuz = new TNode(Escena);
    TNode *NCam = new TNode(Escena);
    TNode *NChasis = new TNode(Escena);
    TraslaLuz->addChild (NLuz);
    TraslaCam->addChild (NCam);
    TraslaCoche->addChild (NChasis);

    TLight *EntLuz = new TLight();
    TCamera *EntCam = new TCamera(45);
    TMesh *MallaChasis = new TMesh();
    NLuz->setEntity(EntLuz);
    NCam->setEntity(EntCam);
    NChasis->setEntity(MallaChasis);

    //---- Recorrer el árbol (dibujarlo) ----
    Escena->draw();

    return 0;

}