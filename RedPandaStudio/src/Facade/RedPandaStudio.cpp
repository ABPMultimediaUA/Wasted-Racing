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
	glUseProgram(scene->getEntity()->getProgramID());
	glEnable(GL_DEPTH_TEST);

	//==================
	//drawShadowMapping();
	//==================

	renderCamera();
	renderLights();

	//scene->draw();

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
	const char * skybox_vertex_path = "shaders/skybox.vert";
	const char * skybox_fragment_path = "shaders/skybox.frag";
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glewExperimental = GL_TRUE;
	std::cout << "GLEW: " << glewGetErrorString(glewInit()) << std::endl;

	//glEnable( GL_DEBUG_OUTPUT );
    //glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, 0 );

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
	initializeShadowMappping();
	//=============================

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

		glm::mat4& view = scene->getEntity()->viewMatrix();
		view = mat;

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

//////////////////////////////////////////////
// GRAPHICS OPTIONS AND PARAMETERS

//================================================= Alexei's magic touch
void RedPandaStudio::initializeShadowMappping()
{
	//Debugging data
	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Generating and binding the frame buffer
	glGenFramebuffers(1, &depthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);    
	
	//Binding and generating texture 2D
	glGenTextures(1, &colorMap);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colorMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	//Binding the render buffer
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer); // now actually attach it

	//Bind created texture to the frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorMap, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	//ERROR Checking
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Que pasadía que no vaya" << std::endl;

	glBindFramebuffer(GL_RENDERBUFFER, 0); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//BASIC STUFFY FOR SHADER CREATION
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

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	GLuint lightProjectionID = glGetUniformLocation(shadowID, "lightProjectionMatrix");
	glUniformMatrix4fv(lightProjectionID, 1, false, &lightProjection[0][0]);
	
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
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	//Push vertex array and the disable it
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
	
	glEnableVertexAttribArray(0); //Vertex points
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1); //Vertex textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//Closing bindings
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Quad texture assignment
	GLuint screenTextureID = glGetUniformLocation(shadowID, "screenTexture");
	glUniform1i(screenTextureID, 0);
}

void RedPandaStudio::drawShadowMapping()
{
	//TESTING THIS SHIET
	// bind to framebuffer and draw scene as we normally would to color texture 
	//glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
	glEnable(GL_DEPTH_TEST);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
	glBindFramebuffer(GL_RENDERBUFFER, renderBuffer);
	glUseProgram(scene->getEntity()->getProgramID());
	renderCamera();
	renderLights();
	scene->draw();

	//bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT); //Clean the dirty color
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded

	//Use our program and the texture given by the frame buffer
	glUseProgram(shadowID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colorMap);	// use the color attachment texture as the texture of the quad plane

	//Bind array buffer and vertex array of data
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0); //Vertex points
	glEnableVertexAttribArray(1); //Vertex texture

	//Draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Unbind all info
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Render scene into the depth buffer
	/*GLuint lightViewID = glGetUniformLocation(shadowID, "lightView");
	GLuint lightModelID = glGetUniformLocation(shadowID, "lightModel");
	glUniformMatrix4fv(lightViewID, 1, false, &lights[0]->getEntity()->viewMatrix()[0][0]);
	glUniformMatrix4fv(lightModelID, 1, false, &lights[0]->getEntity()->modelMatrix()[0][0]);*/


	/*glViewport(0, 0, windowWidth, windowHeight);    //Change viewport size to shadow size
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer); //Choose depthBuffer
		glClear(GL_DEPTH_BUFFER_BIT); 				//Clean screen with buffer

		//paint the thing
		scene->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	// 2. then render scene as normal with shadow mapping (using depth map)
	/*glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->draw();
	glBindTexture(GL_TEXTURE_2D, colorMap);*/
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