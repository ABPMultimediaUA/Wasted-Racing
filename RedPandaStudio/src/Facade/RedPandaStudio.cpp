#include "RedPandaStudio.h"

namespace rps{

RedPandaStudio& RedPandaStudio::createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen){

    static RedPandaStudio rps;

    rps.initSFMLWindow(width, height, depth, framerate, vsync, fullscreen);
    rps.initOpenGL();
    rps.initScene();

    return rps;

}

void RedPandaStudio::dropDevice() {

    window->close();
    delete window;

    //delete scene;
    delete resourceManager;

}

void RedPandaStudio::initSFMLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen) {

    //Specify OpenGL version
    sf::ContextSettings settings = sf::ContextSettings(0,0,0,4.5,0);
    uint32_t style;

    //Specify fullscreen
    if(fullscreen)
        style = sf::Style::Fullscreen;
    else
        style = sf::Style::Default;

    //Create window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "XKating", style, settings);

    window->setFramerateLimit(framerate);
    window->setVerticalSyncEnabled(vsync);

    //Give window to RedPandaStudio
    setWindow(window);

    //Output message
    std::cout << "SFML Window opened..." << std::endl;

}

void RedPandaStudio::initOpenGL() {

    if (glewInit() != GLEW_OK) {
        std::cerr << "Falló al inicializar GLEW\n" << std::endl;
    }
    else {
        std::cout << "GLEW initialized" << std::endl;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);  

    //Pending: start shaders and everything about opengl in here and set program in for the app

    /*GLuint programID = LoadShaders( "test.vertexshader", "test.fragmentshader" );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);  */
    //setProgramID(programID);

}

void RedPandaStudio::initScene() {

    scene = new TNode();
    resourceManager = new ResourceManager();

    //Init matrix and transformations...

    //After this, add functions to add camera and (lights)

    //After this, start testing the engine or even before
    //(try to make some make or something for adapting with the façade)

    //NEVER FORGER INPUT!
}

}