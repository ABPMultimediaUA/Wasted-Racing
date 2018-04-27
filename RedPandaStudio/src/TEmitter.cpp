#include "TEmitter.h"
#include "TResourceMesh.h"
#include <stdlib.h>
#include <time.h> 
#include <climits>

//Simple emitter contructor, half of the parameters are deduced
TEmitter::TEmitter(TResourceMesh* nShape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife,    
            glm::vec3 nBirthDirection, float nBirthSize, glm::vec4 nBirthColor) {

    //Variables relared to particles
    shape   = nShape;              

    position    = nPosition;           
    radius      = nRadius;             

    birthrate       = nBirthrate;          
    particleLife    = nParticleLife;       

    birthDirection      = nBirthDirection;     
    deathDirection      = nBirthDirection;     
    variationDirection  = 0; 

    birthSize           = nBirthSize;          
    deathSize           = nBirthSize;          
    variationSize       = 0;      

    birthColor          = nBirthColor;         
    deathColor          = nBirthColor;         
    variationColor      = 0;     

    //Variables related to emitter  
    particlePool = new Particle*[nBirthrate];
    for(int i = 0; i < nBirthrate; i++){
        particlePool[i] = nullptr;
    }      

    srand(time(nullptr)); 

    particlesSecond = birthrate/particleLife;

}                                 

//Full emitter constructor
TEmitter::TEmitter(TResourceMesh* nShape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife,    
    glm::vec3 nBirthDirection, glm::vec3 nDeathDirection, float nVariationDirection,
    float nBirthSize, float nDeathSize, float nVariationSize,
    glm::vec4 nBirthColor, glm::vec4 nDeathColor, float nVariationColor) {

    //Variables relared to particles
    shape   = nShape;              

    position    = nPosition;           
    radius      = nRadius;             

    birthrate       = nBirthrate;          
    particleLife    = nParticleLife;       

    birthDirection      = nBirthDirection;     
    deathDirection      = nDeathDirection;     
    variationDirection  = nVariationDirection; 

    birthSize           = nBirthSize;          
    deathSize           = nDeathSize;          
    variationSize       = nVariationSize;      

    birthColor          = nBirthColor;         
    deathColor          = nDeathColor;         
    variationColor      = nVariationColor;     

    //Variables related to emitter  
    particlePool = new Particle*[nBirthrate];
    for(int i = 0; i < nBirthrate; i++){
        particlePool[i] = nullptr;
    }       

    srand(time(nullptr)); 

    particlesSecond = birthrate/particleLife;

}

//Delete particle emitter and its particles
TEmitter::~TEmitter() {

    for(int i = 0; i < birthrate; i++){
        if(particlePool[i] != nullptr)
            delete particlePool[i];
    }      
    delete [] particlePool;

}

//Update particle properties
void TEmitter::update(float dTime) {

    float particlesFrame  = particlesSecond * dTime; //Particles generated per frame

    cummulatedFrame += particlesFrame;

    int pendingFrames = (int)cummulatedFrame;
    cummulatedFrame -= pendingFrames;

    for(int i = 0; i < birthrate; i++) {
        //If the particle is dead, delete it
        if(particlePool[i] != nullptr && particlePool[i]->getLife() > particleLife) {
            delete particlePool[i];
            particlePool[i] = nullptr;
        }
        //If the particle is not dead, update it
        else if(particlePool[i] != nullptr && particlePool[i]->getLife() < particleLife) {
            particlePool[i]->update(dTime);
        }
        //If there's no particle and there's particles yet to be emitted, create new particle
        else if(particlePool[i] == nullptr && pendingFrames > 0 && emitting) {

            particlePool[i] = new Particle(this);
            pendingFrames--;
            std::cout << "I ENTER __________________________________________________--" << std::endl;

        }
    }

    std::cout << cummulatedFrame << std::endl;
}

//Stop particle emission (remaining particles are updated until death)
void TEmitter::stop() {
    emitting = false;
} 
//Start normal particle emission             
void TEmitter::play() {
    emitting = true;
}

//Draw current particles               
void TEmitter::draw() {
    for(int i = 0; i <birthrate; i++){
        if(particlePool[i] != nullptr) {
            particlePool[i]->draw();
        }
    }
}            

//Functions to be used by particles
//Set particle vertex data
void TEmitter::setParticleVertexData(GLfloat*& vertex, long & nVertex) {

    vertex = shape->getVertex();
    nVertex = shape->getNVertex();

}
//Set particle life data
void TEmitter::setParticleLifeData(float & nCurrentLife, float & nParticleLife) {

    nCurrentLife = 0;
    nParticleLife = particleLife;

}
//Randomize particle initial position
void TEmitter::setParticlePositionData(glm::vec3 & nPosition) {

    float randX = ((int)rand()) / (float)INT_MAX;
    float randY = ((int)rand()) / (float)INT_MAX;
    float randZ = ((int)rand()) / (float)INT_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;

    nPosition = glm::vec3( position.x + radius*randX*signX, position.y + radius*randY*signY, position.z + radius*randZ*signZ);

}
//Randomize particle direction
void TEmitter::setParticleDirectionData(glm::vec3 & nCurrentDirection, glm::vec3 & nBirthDirection, glm::vec3 & nDeathDirection) {

    float randX = ((int)rand()) / (float)INT_MAX;
    float randY = ((int)rand()) / (float)INT_MAX;
    float randZ = ((int)rand()) / (float)INT_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;

    nCurrentDirection = glm::vec3( birthDirection.x + birthDirection.x*variationDirection*randX*signX, birthDirection.y + birthDirection.y*variationDirection*randY*signY, birthDirection.z + birthDirection.z*variationDirection*randZ*signZ);
    nBirthDirection   = glm::vec3( birthDirection.x + birthDirection.x*variationDirection*randX*signX, birthDirection.y + birthDirection.y*variationDirection*randY*signY, birthDirection.z + birthDirection.z*variationDirection*randZ*signZ);

    randX = ((int)rand()) / (float)INT_MAX;
    randY = ((int)rand()) / (float)INT_MAX;
    randZ = ((int)rand()) / (float)INT_MAX;

    signX = (rand() % 2 == 0) ? 1 : -1;
    signY = (rand() % 2 == 0) ? 1 : -1;
    signZ = (rand() % 2 == 0) ? 1 : -1;

    nDeathDirection   = glm::vec3( deathDirection.x + deathDirection.x*variationDirection*randX*signX, deathDirection.y + deathDirection.y*variationDirection*randY*signY, deathDirection.z + deathDirection.z*variationDirection*randZ*signZ);

}
//Randomize particle size
void TEmitter::setParticleSizeData(float & nCurrentSize, float & nBirthSize, float & nDeathSize) {

    int sign = (rand() % 2 == 0) ? 1 : -1;
    float randX = ((int)rand()) / (float)INT_MAX;

    nBirthSize   = birthSize + birthSize*sign*variationSize*randX;
    nCurrentSize = nBirthSize;

    sign = (rand() % 2 == 0) ? 1 : -1;
    randX = ((int)rand()) / (float)INT_MAX;
    nDeathSize   = deathSize + deathSize*sign*variationSize*randX;

}
//Randomize color
void TEmitter::setParticleColorData(glm::vec4 & nCurrentColor, glm::vec4 & nBirthColor, glm::vec4 & nDeathColor) {

    float randX = ((int)rand()) / (float)INT_MAX;
    float randY = ((int)rand()) / (float)INT_MAX;
    float randZ = ((int)rand()) / (float)INT_MAX;
    float randA = ((int)rand()) / (float)INT_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;
    int signA = (rand() % 2 == 0) ? 1 : -1;

    float r = birthColor.x + 255*signX*variationColor*randX;
    float g = birthColor.y + 255*signY*variationColor*randY;
    float b = birthColor.z + 255*signZ*variationColor*randZ;
    float a = birthColor.w + birthColor.w*signA*variationColor*randA;

    if (r > 255)
        r = 255;
    else if (r < 0)
        r = 0;

    if (g > 255)
        g = 255;
    else if (g < 0)
        g = 0;

    if (b > 255)
        b = 255;
    else if (b < 0)
        b = 0;

    if (a > 1)
        a = 1;
    else if (a < 0)
        a = 0;

    nCurrentColor = glm::vec4(r,g,b,a);
    nBirthColor = glm::vec4(r,g,b,a);

    randX = ((int)rand()) / (float)INT_MAX;
    randY = ((int)rand()) / (float)INT_MAX;
    randZ = ((int)rand()) / (float)INT_MAX;
    randA = ((int)rand()) / (float)INT_MAX;

    signX = (rand() % 2 == 0) ? 1 : -1;
    signY = (rand() % 2 == 0) ? 1 : -1;
    signZ = (rand() % 2 == 0) ? 1 : -1;
    signA = (rand() % 2 == 0) ? 1 : -1;

    r = deathColor.x + 255*signX*variationColor*randX;
    g = deathColor.y + 255*signY*variationColor*randY;
    b = deathColor.z + 255*signZ*variationColor*randZ;
    a = deathColor.w + deathColor.w*signA*variationColor*randA;

    if (r > 255)
        r = 255;
    else if (r < 0)
        r = 0;

    if (g > 255)
        g = 255;
    else if (g < 0)
        g = 0;

    if (b > 255)
        b = 255;
    else if (b < 0)
        b = 0;

    if (a > 1)
        a = 1;
    else if (a < 0)
        a = 0;

    nDeathColor = glm::vec4(r,g,b,a);

}

//Create a particle (given the properties of its emitter)   
TEmitter::Particle::Particle(TEmitter * emitter) {

    emitter->setParticleVertexData(vertex, nVertex);
    emitter->setParticleLifeData(currentLife, particleLife);
    emitter->setParticlePositionData(position);
    emitter->setParticleDirectionData(currentDirection, birthDirection, deathDirection);
    emitter->setParticleSizeData(currentSize, birthSize, deathSize);
    emitter->setParticleColorData(currentColor, birthColor, deathColor);

}      

//Delete particle    
TEmitter::Particle::~Particle() {

}               

//Update particle properties
void TEmitter::Particle::update(float dTime) {

}

//Draw the particle          
void TEmitter::Particle::draw() {

}              
