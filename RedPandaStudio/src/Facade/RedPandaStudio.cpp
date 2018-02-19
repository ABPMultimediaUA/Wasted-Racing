#include "RedPandaStudio.h"

namespace rps{

//////////////////////////////
//  DEVICE CONSTRUCTORS
RedPandaStudio& RedPandaStudio::createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen){

    static RedPandaStudio rps;

    rps.initSFMLWindow(width, height, depth, framerate, vsync, fullscreen);
    rps.initOpenGL();
    rps.initScene();

    return rps;

}

void RedPandaStudio::updateDevice() {

	glUniformMatrix4fv(model, 1, GL_FALSE, &scene->getEntity()->modelMatrix()[0][0]);
    glUniformMatrix4fv(view, 1, GL_FALSE, &scene->getEntity()->viewMatrix()[0][0]);
    glUniformMatrix4fv(projection, 1, GL_FALSE, &scene->getEntity()->projectionMatrix()[0][0]);

	window->clear();
    scene->draw();
	window->setActive();
	window->display();

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
    settings.depthBits = depth;
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

    const char * vertex_file_path = "test.vertexshader";
    const char * fragment_file_path = "test.fragmentshader";

    if (glewInit() != GLEW_OK) {
        std::cerr << "Falló al inicializar GLEW\n" << std::endl;
    }
    else {
        std::cout << "GLEW initialized" << std::endl;
    }

    //Init VBO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);  

    //Create shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read vertex shader from file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Couldn't open %s\n", vertex_file_path);
	}

	//Read fragment shader from file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}else{
		printf("Couldn't open %s\n", vertex_file_path);
	}

    //Init some variables
	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	//Check vertex shader is ok
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	//Check fragment shader is ok
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	//Link OpenGL program using the id
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	//Check the program is ok
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

    //We no longer need the shaders (we have them in the program)
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    //Use the program we have just created
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ProgramID);

    //Give the ProgramID to our engine
    setProgramID(ProgramID);

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	model = glGetUniformLocation(programID, "ModelMatrix");   
    view  = glGetUniformLocation(programID, "ViewMatrix");
    projection = glGetUniformLocation(programID, "ProjectionMatrix");

}

void RedPandaStudio::initScene() {

    scene = new TNode();
    resourceManager = new ResourceManager();


    //Initialize Projection Matrix
    glm::mat4& Projection = scene->getEntity()->projectionMatrix();
    Projection = glm::perspective(glm::radians(45.0f), (float) 16 / (float)9, 1.0f, 100.0f);

    //Initilize View Matrix
    glm::mat4& View = scene->getEntity()->viewMatrix();
    View = glm::lookAt( glm::vec3(-10,15,-3), glm::vec3(0,0,0), glm::vec3(0,1,0) );

    //Initilize Model Matrix
    glm::mat4& Model = scene->getEntity()->modelMatrix();
    Model = glm::mat4(1.0f);
}

//////////////////////////////
//  NODE CONSTRUCTORS
TNode* RedPandaStudio::createObjectNode(TNode* parent, glm::vec3 position, const char* mesh) {

	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr)){

		//Create new transformation
		TTransform* t = new TTransform();
		t->identity();
		t->translate(position.x, position.y, position.z);
		TNode* transform = new TNode(parent, t);

		//Create new mesh entity
		TMesh* m = new TMesh();
		m->setMesh(resourceManager->getResource(mesh));
		TNode* mesh = new TNode(transform, m);

		//Link tree
		transform->addChild(mesh);
		parent->addChild(transform);

		//Return transform
		return transform;
	}
	else{
		return nullptr;
	}
}

TNode* RedPandaStudio::createCamera(TNode* parent, glm::vec3 position) {

	return parent;

}

TNode* RedPandaStudio::createLight(TNode* parent, glm::vec3 position) {

	return parent;

}

//////////////////////////////
//  TRANSFORMATION FACADE
void translateNode(TNode* node, glm::vec3 position) {

	TTransform* t;

	//Check the transform is valid
	if(node != nullptr && (t = dynamic_cast<TTransform*>(node->getEntity())) != nullptr ) {

		t->translate(position.x, position.y, position.z);

	}

}

void rotateNode(TNode* node, float rotation, int axis) {

	TTransform* t;

	//Check the transform is valid
	if(node != nullptr && (t = dynamic_cast<TTransform*>(node->getEntity())) != nullptr ) {

		if(axis == 0)
			t->rotate(1, 0, 0, rotation);
		if(axis == 1)
			t->rotate(0, 1, 0, rotation);
		if(axis == 2)
			t->rotate(0, 0, 1, rotation);
	}

}

void scaleNode(TNode* node, glm::vec3 scale) {

	TTransform* t;

	//Check the transform is valid
	if(node != nullptr && (t = dynamic_cast<TTransform*>(node->getEntity())) != nullptr ) {

		t->scale(scale.x, scale.y, scale.z);

	}

}

}