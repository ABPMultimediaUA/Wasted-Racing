#include "RedPandaStudio.h"

////////////////////////////////
//  ADDITIONAL USEFUL FUNCTIONS
////////////////////////////////

//GUI draw
void (*rpsGUI_draw)() = nullptr;

//Message callback for printing OpenGL errors
void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


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

	//Clean the scene
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Render camera and lights
	renderCamera();
	renderLights();

	//Change shader program for drawing skybox
	glUseProgram(skyboxID);
	glBindVertexArray(skyVertexArray);
	skybox->draw();
	glEnable(GL_DEPTH_TEST);
	
	
	//Activate the billboard shader
	glUseProgram(billboardID);

	//Render all the billboards in the scene
	renderBillboards();

	//Activate the shader used to draw the scene
	glUseProgram(scene->getEntity()->getProgramID());


	//==================
	//drawShadowMapping();
	//==================

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
	//::>>SDL_GL_DeleteContext(&context); // probbly this shouldn't be commented, but it fails otherwise
	SDL_DestroyWindow(window);
	SDL_Quit();
	
    //delete scene;
	deleteNode(scene);
    delete resourceManager;

}

void RedPandaStudio::initSDLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen) {

    // Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
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

	SDL_GLContext ctx = SDL_GL_CreateContext(window);
	context = &ctx;
	glViewport(0, 0, width, height);

    //Give window to RedPandaStudio
    setWindow(window);

    //Output message
    std::cout << "SDL Window opened..." << std::endl;

	//Initilize Scene and ResourceManager here
	scene = new TNode();
    resourceManager = new ResourceManager();


	//=================================================
	//Window size assignation
	windowWidth = width;
	windowHeight = height;
	//=================================================
}

void RedPandaStudio::initOpenGL() {

    const char * vertex_file_path = "shaders/test.vert";
    const char * fragment_file_path = "shaders/test.frag";
	const char * geometry_file_path = "shaders/test.geo";
	const char * skybox_vertex_path = "shaders/skybox.vert";
	const char * skybox_fragment_path = "shaders/skybox.frag";
	const char * billboard_vertex_path = "shaders/billboard.vert";
	const char * billboard_geometry_path = "shaders/billboard.gs";
	const char * billboard_fragment_path = "shaders/billboard.frag";
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glewExperimental = GL_TRUE;
	std::cout << "GLEW: " << glewGetErrorString(glewInit()) << std::endl;


	//Enables the debug output from OpenGL (must be disabled in Release)
	glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, 0 );


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

	//=============================
	//Initialize all parameters needed for the shadow mapping
	//initShadowMappping();
	//=============================

	//Get main shaders
	TResourceShader* vertexShader = resourceManager->getResourceShader(vertex_file_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* geometryShader = resourceManager->getResourceShader(geometry_file_path, (GLenum)GL_GEOMETRY_SHADER);
	TResourceShader* fragmentShader = resourceManager->getResourceShader(fragment_file_path, (GLenum)GL_FRAGMENT_SHADER);

	//Get main shaders ID
	GLuint vertexID = vertexShader->getShaderID();
	GLuint geometryID = geometryShader->getShaderID();
	GLuint fragmentID = fragmentShader->getShaderID();

	//Get skybox shaders
	TResourceShader* skyboxVertex = resourceManager->getResourceShader(skybox_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* skyboxFragment = resourceManager->getResourceShader(skybox_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	//Get skybox shaders ID
	GLuint skyVertexID = skyboxVertex->getShaderID();
	GLuint skyFragmentID = skyboxFragment->getShaderID();

	//Get billboards shaders
	TResourceShader* billboardVertex = resourceManager->getResourceShader(billboard_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* billboardGeometry = resourceManager->getResourceShader(billboard_geometry_path, (GLenum)GL_GEOMETRY_SHADER);
	TResourceShader* billboardFragment = resourceManager->getResourceShader(billboard_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	//Get billboard shaders ID
	GLuint billVertexID = billboardVertex->getShaderID();
	GLuint billGeometryID = billboardGeometry->getShaderID();
	GLuint billFragmentID = billboardFragment->getShaderID();

	//Link OpenGL program using the id
	printf("Linking OpenGL program\n");
	printf("\n");
	printf("\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexID);
	glAttachShader(ProgramID, geometryID);
	glAttachShader(ProgramID, fragmentID);
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
	glDetachShader(ProgramID, vertexID);
	glDetachShader(ProgramID, geometryID);
	glDetachShader(ProgramID, fragmentID);
	
	glDeleteShader(vertexID);
	glDeleteShader(geometryID);
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

	billboardID = glCreateProgram();
	glAttachShader(billboardID, billVertexID);
	glAttachShader(billboardID, billGeometryID);
	glAttachShader(billboardID, billFragmentID);
	glLinkProgram(billboardID);

	//Check the program is ok
	glGetProgramiv(billboardID, GL_LINK_STATUS, &Result);
	glGetProgramiv(billboardID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(billboardID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

    //We no longer need the shaders (we have them in the program)
	glDetachShader(billboardID, billVertexID);
	glDetachShader(billboardID, billGeometryID);
	glDetachShader(billboardID, billFragmentID);
	
	glDeleteShader(billVertexID);
	glDeleteShader(billGeometryID);
	glDeleteShader(billFragmentID);

    //Use the program we have just created
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ProgramID);

    //Give the ProgramID to our engine
    scene->getEntity()->setProgramID(ProgramID);
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
		TResourceOBJ* obj = resourceManager->getResourceOBJ(mesh);
		m->setMesh(obj);
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
		a->setFrames(frames);
		a->setLoop(loop);
		a->setFramerate(framerate);
		TNode* animation = new TNode(transformT, a);
		transformT->addChild(animation);

		//Return mesh
		return animation;
	}
	return nullptr;
}

TNode* RedPandaStudio::createCamera(TNode* parent, glm::vec3 position, glm::vec3 target) {

	//Check parent node is valid
	if(parent != nullptr && parent->getEntity() == nullptr){

		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, glm::vec3(0,0,0));

		//Create new camera entity
		TCamera* c = new TCamera(45.0f);
		TNode* cam = new TNode(transformT, c);
		transformT->addChild(cam);

		//Register camera
		camera = cam;

		//Rotate camera to be behind our character
		updateCamera(position, target);

		//Return camera
		return cam;
	}
	else{
		return nullptr;
	}

}

void RedPandaStudio::updateCamera(glm::vec3 position, glm::vec3 target) {
	
	TTransform* rotation = (TTransform*)camera->getFather()->getFather()->getFather()->getEntity();
	TTransform* translation = (TTransform*)camera->getFather()->getEntity();

	glm::mat4 rot = glm::mat4(1.0);
	glm::mat4 trans = glm::mat4(1.0);

	glm::vec3 f = glm::normalize(target - position);
	glm::vec3 s = glm::normalize(glm::cross(f, glm::vec3(0,1,0)));
	glm::vec3 u = glm::cross(s, f);

	rot[0][0] =  s.x;
	rot[1][0] =  s.y;
	rot[2][0] =  s.z;
	rot[0][1] =  u.x;
	rot[1][1] =  u.y;
	rot[2][1] =  u.z;
	rot[0][2] = -f.x;
	rot[1][2] = -f.y;
	rot[2][2] = -f.z;

	trans[3][0] = -dot(s, position);
	trans[3][1] = -dot(u, position);
	trans[3][2] =  dot(f, position);
	
	rotation->setMatrix(rot);
	translation->setMatrix(trans);
}

TNode* RedPandaStudio::createLight(TNode* parent, glm::vec3 position, glm::vec3 intensity) 
{

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

TNode* RedPandaStudio::createSpotlight(TNode* parent, glm::vec3 position, glm::vec3 intensity, glm::vec3 direction, float cutoff)
{
	//Check parent node is valid
	if(parent != nullptr && (parent->getEntity() == nullptr || dynamic_cast<TTransform*>(parent->getEntity()) != nullptr))
	{
		//Create new transformation tree
		TNode* transformT = addRotScaPos(parent, position);

		//Create new light entity
		TSpotlight* l = new TSpotlight(intensity, direction, cutoff);
		TNode* light = new TNode(transformT, l);
		transformT->addChild(light);

		//Register light
		spotlights.push_back(light);

		//Return light
		return light;
	}
	else
	{
		return nullptr;
	}
}

TBillboard* RedPandaStudio::createBillboard(char* n, glm::vec3 p)
{
	//Create or get a reference to the texture from the resourceManager
	TResourceTexture* t = resourceManager->getResourceTexture(n);
	
	//Create the billboard
	TBillboard* b = new TBillboard(t, p);
	billboards.push_back(b);

	return b;
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
		(t = dynamic_cast<TLight*>(leaf->getEntity())) != nullptr  ||
		(t = dynamic_cast<TAnimation*>(leaf->getEntity())) != nullptr)) {

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
void RedPandaStudio::renderLights() 
{

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

	
	for(unsigned int i = 0; i < spotlights.size(); i++)
	{
		glm::mat4 mat = glm::mat4(1.0);
		calculateNodeTransform(spotlights[i], mat);

		glm::vec4 pos = mat * glm::vec4(0.0, 0.0, 0.0, 1.0);
		TSpotlight* l = (TSpotlight*)spotlights[i]->getEntity();

		std::string lightName = std::string("spotlight[" +  std::to_string(i) + "].light.position");
		GLuint posID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName.c_str());
		glUniform4fv(posID, 1, &pos[0]);

		std::string lightName2 = std::string("spotlight[" + std::to_string(i) + "].light.intensity");
		GLuint intID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName2.c_str());
		glUniform4fv(intID, 1, &l->getIntensity()[0]);

		std::string lightName3 = std::string("spotlight[" + std::to_string(i) + "].direction");
		GLuint dirID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName3.c_str());
		glUniform3fv(dirID, 1, &l->getDirection()[0]);

		std::string lightName4 = std::string("spotlight[" + std::to_string(i) + "].cutoff");
		GLuint cutID = glGetUniformLocation(scene->getEntity()->getProgramID(), lightName4.c_str());
		glUniform1f(cutID, l->getCutoff());
	}

	GLuint numL = glGetUniformLocation(scene->getEntity()->getProgramID(), "numLights");
	glUniform1i(numL, lights.size());

	GLuint numSL = glGetUniformLocation(scene->getEntity()->getProgramID(), "numSpotLights");
	glUniform1i(numSL, spotlights.size());
}
void RedPandaStudio::renderCamera() 
{

	if(camera != nullptr)
	{
		glm::mat4 mat = glm::mat4(1.0);
		calculateNodeTransform(camera, mat);

		glm::mat4& view = scene->getEntity()->viewMatrix();
		view = mat;

		glUniformMatrix4fv(scene->getEntity()->getViewID(), 1, GL_FALSE, &scene->getEntity()->viewMatrix()[0][0]);
    	glUniformMatrix4fv(scene->getEntity()->getProjectionID(), 1, GL_FALSE, &scene->getEntity()->projectionMatrix()[0][0]);
	}
}

void RedPandaStudio::renderBillboards()
{
	glm::mat4 v = scene->getEntity()->viewMatrix();
	glm::mat4 p = scene->getEntity()->projectionMatrix();
	glm::mat4 m = p * v;
	glm::vec3 camPos = glm::vec3(-v[3][2], -v[3][1], -v[3][0]);

	GLuint VPMat = glGetUniformLocation(billboardID, "VPMatrix");
	glUniformMatrix4fv(VPMat, 1, GL_FALSE, &m[0][0]);

	GLuint cameraPosition = glGetUniformLocation(billboardID, "cameraPosition");
	glUniform3fv(cameraPosition, 1, &camPos[0]);

	for(int i = 0; i < billboards.size(); i++)
	{
		billboards[i]->beginDraw();
	}
}

//Recursive function. Should receive an identity as input. Returns the accumulated transformation
void RedPandaStudio::calculateNodeTransform(TNode* node, glm::mat4& mat) 
{

	if(node!= nullptr && node->getFather() != nullptr) 
	{

		TTransform* t = dynamic_cast<TTransform*>(node->getEntity());
		if( t != nullptr) 
			mat *= t->getMatrix(); 

		calculateNodeTransform(node->getFather(), mat);
	}
}

//////////////////////////////////////////////
// GRAPHICS OPTIONS AND PARAMETERS

//================================================= Alexei's magic touch
void RedPandaStudio::initPostProcessing()
{
	//Debugging data
	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Generating and binding the frame buffer
	glGenFramebuffers(1, &postProcessingBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingBuffer);
	
	//Binding and generating texture 2D which stores the depth data to render later
	glGenTextures(1, &colorMap);
	glBindTexture(GL_TEXTURE_2D, colorMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	//Bind created texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap, 0);

	//Binding the render buffer. Even if we don't use it, it's needed because otherwise the image gets corrupted without a depth
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer); // now actually attach it
	
	//ERROR Checking
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Que pasadía que no vaya" << std::endl;

	glBindFramebuffer(GL_RENDERBUFFER, 0); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout<<"Buffers y texturas creados"<<std::endl;

	//BASIC STUFFY FOR SHADER CREATION
	const char *processing_fragment_path = "shaders/postProcessing.frag";
	const char *processing_vertex_path = "shaders/postProcessing.vert";

	TResourceShader* processingVertex = resourceManager->getResourceShader(processing_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* processingragment = resourceManager->getResourceShader(processing_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	GLuint processingVertexID = processingVertex->getShaderID();
	GLuint processingFragmentID = processingragment->getShaderID();

	processingID = glCreateProgram();
	glAttachShader(processingID, processingVertexID);
	glAttachShader(processingID, processingFragmentID);
	glLinkProgram(processingID);
	std::cout << "Linking processingID" << std::endl;

	//Check the program is ok
	glGetProgramiv(processingID, GL_LINK_STATUS, &Result);
	glGetProgramiv(processingID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(processingID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//Delete because the program is cool
	glDetachShader(processingID,processingVertexID);
	glDetachShader(processingID,processingFragmentID);
	glDeleteShader(processingVertexID);
	glDeleteShader(processingFragmentID);

	glUseProgram(processingID); //REQUIRED for attaching variables
	
	//Quad in which we'll paint the scene
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    //Bind the array to the array buffer
    glGenBuffers(1, &processingQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, processingQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	//Push vertex array and the disable it
    glGenVertexArrays(1, &processingQuadVAO);
    glBindVertexArray(processingQuadVAO);
	
	glEnableVertexAttribArray(3); //Vertex points
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(4); //Vertex textures
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//Closing bindings
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Quad texture assignment
	postprocessing_sampler = glGetUniformLocation(processingID, "screenTexture");
}


void RedPandaStudio::drawPostProcessing()
{
	//Bind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingBuffer);     //Binding where we'll store our image
	glEnable(GL_DEPTH_TEST);						      //Enabling for 3D renders
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //Cleaning the buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 			      //Clean window
 
	//Draw the scene normally
	glUseProgram(scene->getEntity()->getProgramID());
	renderCamera();
	renderLights();
	scene->draw();

	//bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT); //Clean the dirty color
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded

	//Use our program and the texture given by the frame buffer
	glUseProgram(processingID);

	//Bind array buffer and vertex array of data
    glBindBuffer(GL_ARRAY_BUFFER, processingQuadVBO);
    glBindVertexArray(processingQuadVAO);
	glEnableVertexAttribArray(3); //Vertex points
	glEnableVertexAttribArray(4); //Vertex texture

	//Bind the texture where we draw the scene
	glBindTexture(GL_TEXTURE_2D, colorMap);	//use the color attachment texture as the texture of the quad plane
	glUniform1i(postprocessing_sampler, 0);  	 	//ID assignment for the fragment shader (using GL_TEXTURE1 to avoid collisions)

	//Draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Unbind all info
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RedPandaStudio::initShadowMappping()
{
	//Debugging data
	GLint Result = GL_FALSE;
	int InfoLogLength;
	
	//Generating and binding the frame buffer
	glGenFramebuffers(1, &depthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);

	//Set this to GL_NONE so no color data is written in the frame buffer. We only want the depth data.
	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
	
	//Binding and generating texture 2D which stores the depth data to render later
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	
	//Bind created texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	//ERROR Checking
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Que pasadía que no vaya" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout<<"Buffers y texturas creados"<<std::endl;

	//==========================================
	///TESTING::: the quad shader
	//==========================================

	const char *shadowMapQuad_fragment_path = "shaders/shadowMapQuad.frag";
	const char *shadowMapQuad_vertex_path = "shaders/shadowMapQuad.vert";

	TResourceShader* shadowQuadVertex = resourceManager->getResourceShader(shadowMapQuad_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* shadowQuadFragment = resourceManager->getResourceShader(shadowMapQuad_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	GLuint shadowQuadVertexID = shadowQuadVertex->getShaderID();
	GLuint shadowQuadFragmentID = shadowQuadFragment->getShaderID();

	shadowQuadID = glCreateProgram();
	glAttachShader(shadowQuadID, shadowQuadVertexID);
	glAttachShader(shadowQuadID, shadowQuadFragmentID);
	glLinkProgram(shadowQuadID);
	std::cout << "Linking shadowQuadID" << std::endl;

	//Check the program is ok
	glGetProgramiv(shadowQuadID, GL_LINK_STATUS, &Result);
	glGetProgramiv(shadowQuadID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shadowQuadID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//Delete because the program is cool
	glDetachShader(shadowQuadID,shadowQuadVertexID);
	glDetachShader(shadowQuadID,shadowQuadFragmentID);
	glDeleteShader(shadowQuadVertexID);
	glDeleteShader(shadowQuadFragmentID);

	//==========================================
	// Shadow map Shader
	//==========================================
	const char *shadowMap_fragment_path = "shaders/shadowMap.frag";
	const char *shadowMap_vertex_path = "shaders/shadowMap.vert";

	TResourceShader* shadowVertex = resourceManager->getResourceShader(shadowMap_vertex_path, (GLenum)GL_VERTEX_SHADER);
	TResourceShader* shadowFragment = resourceManager->getResourceShader(shadowMap_fragment_path, (GLenum)GL_FRAGMENT_SHADER);

	GLuint shadowVertexID = shadowVertex->getShaderID();
	GLuint shadowFragmentID = shadowFragment->getShaderID();

	shadowID = glCreateProgram();
	glAttachShader(shadowID, shadowVertexID);
	glAttachShader(shadowID, shadowFragmentID);
	glLinkProgram(shadowID);
	std::cout << "Linking shadowID" << std::endl;

	//Check the program is ok
	glGetProgramiv(shadowID, GL_LINK_STATUS, &Result);
	glGetProgramiv(shadowID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shadowID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//Delete because the program is cool
	glDetachShader(shadowID,shadowVertexID);
	glDetachShader(shadowID,shadowFragmentID);
	glDeleteShader(shadowVertexID);
	glDeleteShader(shadowFragmentID);

	glUseProgram(shadowID); //REQUIRED for attaching variables
	
	//Quad in which we'll paint the scene
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    //Bind the array to the array buffer
    glGenBuffers(1, &shadowQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, shadowQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	//Push vertex array and the disable it
    glGenVertexArrays(1, &shadowQuadVAO);
    glBindVertexArray(shadowQuadVAO);
	
	glEnableVertexAttribArray(3); //Vertex points
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(4); //Vertex textures
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//Closing bindings
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Quad texture assignment
	shadow_sampler = glGetUniformLocation(shadowID, "screenTexture");
}

void RedPandaStudio::drawShadowMapping()
{
	//=========================
	//Data and states preparation

	//Backup data
	GLuint programID = scene->getEntity()->getProgramID();
	GLuint restoreModel = glGetUniformLocation(programID, "ModelMatrix");

	//Secundary sampler2D
	shadowMap_sampler = glGetUniformLocation(programID, "shadowMap");
	glUniform1i(shadowMap_sampler, 0);

	//Bind the buffer
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);     //Binding where we'll store our image
	glEnable(GL_DEPTH_TEST);						    //Enabling for 3D renders
	glClear( GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );//Cleaning the buffers
 
	//Testeo de escena normal
	

	/*glUseProgram(programID);
	renderCamera();
	renderLights();
	scene->draw();
*/
	

	//Use shadow program to render the depth
	glUseProgram(shadowID);

	//=========================
	//Calculate light variables
	glm::mat4 mat = glm::mat4(1.0);
	calculateNodeTransform(lights[0], mat);

	glm::vec4 pos = mat * glm::vec4(0.0, 0.0, 0.0, 1.0);
    glm::mat4 lightView = glm::lookAt(
                                        glm::vec3(pos), 	//Eye: light position
                                        glm::vec3(0, 0, 0), //Center: object position
                                    	glm::vec3(0, 1, 0)  //Up: Y¹
                                     );

	GLuint lightModelID = glGetUniformLocation(shadowID, "lightModel");
	scene->getEntity()->setModelID(lightModelID);

	float near_plane = 0.0000001f, far_plane = 7.5f;
	glm::mat4 lightProView = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane) * lightView;

	GLuint lightProViewID = glGetUniformLocation(shadowID, "lightProView");
	glUniformMatrix4fv(lightProViewID, 1, false, &lightProView[0][0]);

	//Draw the scene under this context
	scene->draw();

	//bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//=========================
	//QUAD CHECKER
/*
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT); //Clean the dirty color
	glDisable(GL_DEPTH_TEST);     // disable depth test so screen-space quad isn't discarded

	//Use our program and the texture given by the frame buffer
	glUseProgram(shadowQuadID);

	//Bind array buffer and vertex array of data
    glBindBuffer(GL_ARRAY_BUFFER, shadowQuadVBO);
    glBindVertexArray(shadowQuadVAO);
	glEnableVertexAttribArray(3); //Vertex points
	glEnableVertexAttribArray(4); //Vertex texture

	//Bind the texture where we draw the scene
	glBindTexture(GL_TEXTURE_2D, depthMap);	//use the color attachment texture as the texture of the quad plane
	glUniform1i(shadow_sampler, 0);  	 	//ID assignment for the fragment shader (using GL_TEXTURE1 to avoid collisions)

	//Draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Unbind all info
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	//Restore data
	scene->getEntity()->setModelID(restoreModel);
	scene->getEntity()->setProgramID(programID);

	//Render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID);

	//Passing shadow variables
	GLuint lightSpaceViewID = glGetUniformLocation(programID, "lightSpaceView");
	glUniformMatrix4fv(lightSpaceViewID, 1, false, &lightProView[0][0]);

	//Texture 0: object, Texture 1: shadow map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glActiveTexture(GL_TEXTURE0);

	//Normal render
	renderLights();
	renderCamera();
	scene->draw();
}
//=================================================


void RedPandaStudio::setCulling(bool b, GLenum e)
{
	if(b)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(e);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}


//Add lod object
void RedPandaStudio::addMeshLoD(int lvl, const char* mesh)
{
	if(mesh != "" && lvl > 0)
	{
		TResourceLoD* lod = resourceManager->getResourceLoD(mesh);

		std::string route = "";
		int x = 0;

		while(mesh[x]!='.'){
			route += mesh[x];
			x++;
		}

		for(int i = 1; i <= lvl; i++)
		{
			std::string m = route+std::to_string(i);
			std::string o = ".obj";
			m += o;
			TResourceOBJ* obj = resourceManager->getResourceOBJ(m.c_str());
			lod->insertObj(i, obj);
		}
	}
}

//////////////////////////////
//  TRANSFORMATION FACADE
void translateNode(TNode* node, glm::vec3 position) {

	TEntity* t;

	//Check the input is a mesh, camera or light
	if(node != nullptr && ((t = dynamic_cast<TMesh*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TCamera*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr  ||
		(t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)) {

		TTransform* tr = (TTransform*)node->getFather()->getEntity();

		tr->identity();
		tr->translate(position.x, position.y, position.z);

	}

}

void scaleNode(TNode* node, glm::vec3 scale) {

	TEntity* t;

	//Check the input is a mesh, camera or light
	if(node != nullptr && ((t = dynamic_cast<TMesh*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TCamera*>(node->getEntity())) != nullptr ||
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr  ||
		(t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)) {

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
		(t = dynamic_cast<TLight*>(node->getEntity())) != nullptr  ||
		(t = dynamic_cast<TAnimation*>(node->getEntity())) != nullptr)) {

		TTransform* tr = (TTransform*)node->getFather()->getFather()->getFather()->getEntity();

		tr->identity();
		tr->rotate(0, 1, 0, rotation.y);
		tr->rotate(1, 0, 0, rotation.x);
		tr->rotate(0, 0, 1, rotation.z);
	}

}


}