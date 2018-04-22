#include "RedPandaStudio.h"

//GUI drawing function adress
void (*rpsGUI_draw)() = nullptr;

namespace rps{

//////////////////////////////
//  DEVICE CONSTRUCTORS
//////////////////////////////

RedPandaStudio& RedPandaStudio::createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen){

    static RedPandaStudio rps;

    rps.initSDLWindow(width, height, depth, framerate, vsync, fullscreen);
    rps.initOpenGL();
    rps.initScene();
	
    return rps;

}

void RedPandaStudio::updateDevice() {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	renderCamera();
	renderLights();

	//Change shader program for drawing skybox
	glUseProgram(skyboxID);
	glBindVertexArray(skyVertexArray);
	skybox->draw();
	glUseProgram(scene->getEntity()->getProgramID());
	glEnable(GL_DEPTH_TEST);

	renderCamera();
	renderLights();

	scene->draw();

	if(rpsGUI_draw != nullptr)
		rpsGUI_draw();

	SDL_GL_SwapWindow(window);

	//Chrono to update the fps value
	std::chrono::time_point<std::chrono::high_resolution_clock> currTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = currTime - lastTime;
	lastTime = currTime;

	double fps = 1/elapsed.count();
	
	if(showFPS)
	{
		std::cout << "FPS: " << fps << std::endl;
	}
}

void RedPandaStudio::setGUIDrawFunction(void (*f)()) {
	rpsGUI_draw = f;
}

void RedPandaStudio::dropDevice() {

	// Delete our OpengL context
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

    //delete scene;
	deleteNode(scene);
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
	glViewport(0, 0, width, height);

    //Give window to RedPandaStudio
    setWindow(window);

    //Output message
    std::cout << "SDL Window opened..." << std::endl;

	//Initilize Scene and ResourceManager here
	scene = new TNode();
    resourceManager = new ResourceManager();

}

void RedPandaStudio::initOpenGL() {

    const char * vertex_file_path = "shaders/test.vert";
    const char * fragment_file_path = "shaders/test.frag";
	const char * skybox_vertex_path = "shaders/skybox.vert";
	const char * skybox_fragment_path = "shaders/skybox.frag";
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glewExperimental = GL_TRUE;
	std::cout << "GLEW: " << glewGetErrorString(glewInit()) << std::endl;

    //Init VBO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

	glGenVertexArrays(1, &skyVertexArray);
	glDepthFunc(GL_LEQUAL);
	
	//Init skybox
	skybox = new TResourceSkybox();
	char* r0 = "media/img/darkskies_ft.tga";
	char* r1 = "media/img/darkskies_bk.tga";
	char* r2 = "media/img/darkskies_up.tga";
	char* r3 = "media/img/darkskies_dn.tga";
	char* r4 = "media/img/darkskies_rt.tga";
	char* r5 = "media/img/darkskies_lf.tga";
	
	skybox->loadResource(r0, 0);
	skybox->loadResource(r1, 1);
	skybox->loadResource(r2, 2);
	skybox->loadResource(r3, 3);
	skybox->loadResource(r4, 4);
	skybox->loadResource(r5, 5);

	skybox->initSkybox();

	//Get main shaders
	TResourceShader* vertexShader = resourceManager->getResourceShader(vertex_file_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* fragmentShader = resourceManager->getResourceShader(fragment_file_path, (GLenum)GL_FRAGMENT_SHADER);

	//Get main shaders ID
	GLuint vertexID = vertexShader->getShaderID();
	GLuint fragmentID = fragmentShader->getShaderID();

	//Get skybox shaders
	TResourceShader* skyboxVertex = resourceManager->getResourceShader(skybox_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* skyboxFragment = resourceManager->getResourceShader(skybox_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	//Get skybox shaders ID
	GLuint skyVertexID = skyboxVertex->getShaderID();
	GLuint skyFragmentID = skyboxFragment->getShaderID();

	//Link OpenGL program using the id
	printf("Linking OpenGL program\n");
	printf("\n");
	printf("\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexID);
	glAttachShader(ProgramID, fragmentID);
	glLinkProgram(ProgramID);

    //We no longer need the shaders (we have them in the program)
	glDetachShader(ProgramID, vertexID);
	glDetachShader(ProgramID, fragmentID);
	
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	//Create Skybox program
	skyboxID = glCreateProgram();
	glAttachShader(skyboxID, skyVertexID);
	glAttachShader(skyboxID, skyFragmentID);
	glLinkProgram(skyboxID);

	//Check the program is ok
	glGetProgramiv(skyboxID, GL_LINK_STATUS, &Result);
	glGetProgramiv(skyboxID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(skyboxID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//We no longer need the shaders (we have them in the program)
	glDetachShader(skyboxID, skyVertexID);
	glDetachShader(skyboxID, skyFragmentID);
	
	glDeleteShader(skyVertexID);
	glDeleteShader(skyFragmentID);

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

	GLuint viewSky = glGetUniformLocation(skyboxID, "ViewMatrix");
	skybox->setView(viewSky);

}

void RedPandaStudio::initScene() {

    //Initialize Projection Matrix
    glm::mat4& Projection = scene->getEntity()->projectionMatrix();
    Projection = glm::mat4(1.0f);

    //Initilize View Matrix
    glm::mat4& View = scene->getEntity()->viewMatrix();
    View = glm::mat4(1.0f);

    //Initilize Model Matrix
    glm::mat4& Model = scene->getEntity()->modelMatrix();
    Model = glm::mat4(1.0f);

	lastTime = std::chrono::high_resolution_clock::now();	
}

//////////////////////////////
//  NODE CONSTRUCTORS
TNode* RedPandaStudio::createObjectNode(TNode* parent, glm::vec3 pos, const char* mesh) {

	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr))
	{

		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, pos);

		//Create new mesh entity
		TMesh* m = new TMesh();
		m->setMesh(resourceManager->getResourceOBJ(mesh));
		TNode* mesh = new TNode(transformT, m);
		transformT->addChild(mesh);

		//Return mesh
		return mesh;
	}
	else{
		return nullptr;
	}
}

TNode* RedPandaStudio::createAnimatedNode(TNode* parent, glm::vec3 pos, const char* animation, bool loop, int frames, double framerate)
{
	
	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr))
	{
		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, pos);

		//Create new mesh entity
		TAnimation* a = new TAnimation();
		a->setAnimation(resourceManager->getResourceAnimation(animation, frames));
		a->getAnimation()->setLoop(loop);
		a->getAnimation()->setFramerate(framerate);
		TNode* animation = new TNode(transformT, a);
		transformT->addChild(animation);

		//Return mesh
		return animation;
	}
	return nullptr;
}

TNode* RedPandaStudio::createCamera(TNode* parent, glm::vec3 position) {

	//Check parent node is valid
	if(parent != nullptr && dynamic_cast<TTransform*>(parent->getEntity()) != nullptr){

		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, position);

		//Create new camera entity
		TCamera* c = new TCamera(45.0f);
		TNode* cam = new TNode(transformT, c);
		transformT->addChild(cam);

		//Register camera
		camera = cam;

		//Rotate camera to be behind our character
		rps::rotateNode(camera,glm::vec3(0,glm::half_pi<float>(),0));

		//Return camera
		return cam;
	}
	else{
		return nullptr;
	}

}

TNode* RedPandaStudio::createLight(TNode* parent, glm::vec3 position, glm::vec3 intensity) {

	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr)){

		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, position);

		//Create new light entity
		TLight* l = new TLight(intensity);
		TNode* light = new TNode(transformT, l);
		transformT->addChild(light);

		//Register light
		lights.push_back(light);

		//Return light
		return light;
	}
	else{
		return nullptr;
	}

}

void RedPandaStudio::deleteObject(TNode* leaf) {

	TEntity* t;

	//Unregister camera
	if(leaf != nullptr && (t = dynamic_cast<TCamera*>(leaf->getEntity())) != nullptr){

		camera = nullptr;

	} //Unregister lights
	if(leaf != nullptr && (t = dynamic_cast<TLight*>(leaf->getEntity())) != nullptr){

		for(unsigned int i = 0; i < lights.size(); i++){

			if(lights[i] == leaf)
				lights.erase(lights.begin() + i);
		}
	}

	if(leaf != nullptr && ((t = dynamic_cast<TMesh*>(leaf->getEntity())) != nullptr ||
		(t = dynamic_cast<TCamera*>(leaf->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(leaf->getEntity())) != nullptr)) {

			TNode* first = leaf->getFather()->getFather()->getFather();
			TNode* parent = leaf->getFather()->getFather()->getFather()->getFather();

			deleteNode(first);

			//Once deleted the object, erase the object from his parent child list
			parent->removeChild(first);
	}

}

void RedPandaStudio::deleteNode(TNode* node) {

	if(node != nullptr){

		std::vector<TNode*> children = node->getChild();

		if(children.size() == 0){
			delete node;
		}
		else{
			for(unsigned int i = 0; i < children.size(); i++){
				deleteNode(children[i]);
			}
			delete node;
		}
	}
}

TNode* RedPandaStudio::addRotScaPos(TNode* parent, glm::vec3 pos) {

		//Rotation transformation
		TTransform* tR = new TTransform();
		tR->identity();
		TNode* transformR = new TNode(parent, tR);
		parent->addChild(transformR);

		//Scale transformation
		TTransform* tS = new TTransform();
		tS->identity();
		TNode* transformS = new TNode(transformR, tS);
		transformR->addChild(transformS);

		//Translation transformation
		TTransform* tT = new TTransform();
		tT->identity();
		tT->translate(pos.x, pos.y, pos.z);
		TNode* transformT = new TNode(transformS, tT);
		transformS->addChild(transformT);

		return transformT;

}

////////////////////////////////////
//  LIGHTS AND CAMERA MANAGEMENT
void RedPandaStudio::renderLights() {

	for(unsigned int i = 0; i < lights.size(); i++){

		glm::mat4 mat = glm::mat4(1.0);
		calculateNodeTransform(lights[i], mat);

		glm::vec4 pos = mat * glm::vec4(0.0, 0.0, 0.0, 1.0);

		std::string lightName = std::string("light[" + std::to_string(i) + "].position");
		GLuint posID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName.c_str());
		glUniform4fv(posID, 1, &pos[0]);

		std::string lightName2 = std::string("light[" + std::to_string(i) + "].intensity");
		GLuint intID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName2.c_str());
		TLight* l = (TLight*)lights[i]->getEntity();
		glUniform4fv(intID, 1, &l->getIntensity()[0]);
	}

	GLuint numL = glGetUniformLocation(scene->getEntity()->getProgramID(), "numLights");
	glUniform1i(numL, lights.size());

}
void RedPandaStudio::renderCamera() {

	if(camera != nullptr){
		glm::mat4 mat = glm::mat4(1.0);
		calculateNodeTransform(camera, mat);


		//Get camera specific transformations
		TTransform* t1 = (TTransform*) camera->getFather()->getFather()->getFather()->getFather()->getEntity();	//distance from center
		TTransform* r = (TTransform*) camera->getFather()->getFather()->getFather()->getFather()->getFather()->getFather()->getEntity();	//parent rotation
		TTransform* r1 = (TTransform*) camera->getFather()->getFather()->getFather()->getEntity();	//camera rotation
		TTransform* t0 = (TTransform*) camera->getFather()->getEntity();	//distance from player

		glm::mat4& view = scene->getEntity()->viewMatrix();
    	view = glm::inverse(t1->getMatrix() * r->getMatrix() * r1->getMatrix() * t0->getMatrix());

		glUniformMatrix4fv(scene->getEntity()->getViewID(), 1, GL_FALSE, &scene->getEntity()->viewMatrix()[0][0]);
    	glUniformMatrix4fv(scene->getEntity()->getProjectionID(), 1, GL_FALSE, &scene->getEntity()->projectionMatrix()[0][0]);
	}
}

//Recursive function. Should receive an identity as input. Returns the accumulated transformation
void RedPandaStudio::calculateNodeTransform(TNode* node, glm::mat4& mat) {

	if(node!= nullptr && node->getFather() != nullptr) {

		TTransform* t = dynamic_cast<TTransform*>(node->getEntity());
		if( t != nullptr) 
			mat *= t->getMatrix(); 

		calculateNodeTransform(node->getFather(), mat);
	}
}

//////////////////////////////
//  TRANSFORMATION FACADE
void translateNode(TNode* node, glm::vec3 position) {

	TEntity* t;

	//Check the input is a mesh, camera or light
	if(node != nullptr && ((t = dynamic_cast<TMesh*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TCamera*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr)) {

		TTransform* tr = (TTransform*)node->getFather()->getEntity();

		tr->identity();
		tr->translate(position.x, position.y, position.z);

	}

}

void scaleNode(TNode* node, glm::vec3 scale) {

	TEntity* t;

	//Check the input is a mesh, camera or light
	if(node != nullptr && ((t = dynamic_cast<TMesh*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr || 
		(t = dynamic_cast<TCamera*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr)) {

		TTransform* tr = (TTransform*)node->getFather()->getFather()->getEntity();


		tr->identity();
		tr->scale(scale.x, scale.y, scale.z);
	}

}

void rotateNode(TNode* node, glm::vec3 rotation) {

	TEntity* t;

	//Check the input is a mesh, camera or light
	if(node != nullptr && ((t = dynamic_cast<TMesh*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TCamera*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr)) {

		TTransform* tr = (TTransform*)node->getFather()->getFather()->getFather()->getEntity();

		tr->identity();
		tr->rotate(0, 1, 0, rotation.y);
		tr->rotate(1, 0, 0, rotation.x);
		tr->rotate(0, 0, 1, rotation.z);
	}

}

void loopAnimation(TNode* node) {

	TAnimation* t;

	if(node != nullptr && ((t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)){

		TResourceAnimation* anim = t->getAnimation();

		if(anim != nullptr) {

			anim->setLoop(true);
		}
	}
}

void pauseAnimation(TNode* node) {

	TAnimation* t;

	if(node != nullptr && ((t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)){

		TResourceAnimation* anim = t->getAnimation();

		if(anim != nullptr) {

			anim->setPause(true);
		}
	}
}

void playOnceAnimation(TNode* node) {

	TAnimation* t;

	if(node != nullptr && ((t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)){

		TResourceAnimation* anim = t->getAnimation();

		if(anim != nullptr) {

			anim->playNoLoop();
		}
	}
}

void updateAnimation(TNode* node, double dTime) {

	TAnimation* t;

	if(node != nullptr && ((t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)){

		TResourceAnimation* anim = t->getAnimation();

		if(anim != nullptr) {

			anim->update(dTime);
		}
	}

}

}