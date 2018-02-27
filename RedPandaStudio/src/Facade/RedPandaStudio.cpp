#include "RedPandaStudio.h"

namespace rps{

//////////////////////////////
//  DEVICE CONSTRUCTORS
RedPandaStudio& RedPandaStudio::createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen){

    static RedPandaStudio rps;

    rps.initSDLWindow(width, height, depth, framerate, vsync, fullscreen);
    rps.initOpenGL();
    rps.initScene();

    return rps;

}

void RedPandaStudio::updateDevice() {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUniformMatrix4fv(scene->getEntity()->getModelID(), 1, GL_FALSE, &scene->getEntity()->modelMatrix()[0][0]);
    glUniformMatrix4fv(scene->getEntity()->getViewID(), 1, GL_FALSE, &scene->getEntity()->viewMatrix()[0][0]);
    glUniformMatrix4fv(scene->getEntity()->getProjectionID(), 1, GL_FALSE, &scene->getEntity()->projectionMatrix()[0][0]);

	scene->draw();

	SDL_GL_SwapWindow(window);

}

void RedPandaStudio::dropDevice() {

	// Delete our OpengL context
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

    //delete scene;
    delete resourceManager;

}

void RedPandaStudio::initSDLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen) {

    // Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	  std::cerr << "Couldn't init SDL" << std::endl;
	SDL_GL_LoadLibrary(NULL);

	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);

	// Create the window
	if (fullscreen) {
	  window = SDL_CreateWindow(
	    "XKating", 
	    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
	    0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
	  );
	} else {
	  window = SDL_CreateWindow(
	    "XKating", 
	    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
	    width, height, SDL_WINDOW_OPENGL
	  );
	}

	if(vsync){
		SDL_GL_SetSwapInterval(1);
	}

	context = SDL_GL_CreateContext(window);

    //Give window to RedPandaStudio
    setWindow(window);

    //Output message
    std::cout << "SDL Window opened..." << std::endl;

	//Initilize Scene and ResourceManager here
	scene = new TNode();
    resourceManager = new ResourceManager();

}

void RedPandaStudio::initOpenGL() {

    const char * vertex_file_path = "test.vertexshader";
    const char * fragment_file_path = "test.fragmentshader";

	glewExperimental = GL_TRUE;

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
    scene->getEntity()->setProgramID(ProgramID);

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	GLuint model = glGetUniformLocation(ProgramID, "ModelMatrix");   
    GLuint view  = glGetUniformLocation(ProgramID, "ViewMatrix");
    GLuint projection = glGetUniformLocation(ProgramID, "ProjectionMatrix");
	scene->getEntity()->setModelID(model);
	scene->getEntity()->setViewID(view);
	scene->getEntity()->setProjectionID(projection);

}

void RedPandaStudio::initScene() {

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
TNode* RedPandaStudio::createObjectNode(TNode* parent, glm::vec3 position, const char* mesh, const char* text) {

	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr)){

		//Create new transformation
		TTransform* t = new TTransform();
		t->identity();
		t->translate(position.x, position.y, position.z);
		TNode* transform = new TNode(parent, t);

		//Create new mesh entity
		TMesh* m = new TMesh();
		m->setMesh(resourceManager->getResourceMesh(mesh));
		m->setTexture(resourceManager->getResourceTexture(text));
		m->setTextActive(true);
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