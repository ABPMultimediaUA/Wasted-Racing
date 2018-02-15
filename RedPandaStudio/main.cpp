#include <RedPanda.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Crear los shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Leer el Vertex Shader desde archivo
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Leer el Fragment Shader desde archivo
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compilar Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Revisar Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compilar Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Revisar Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Vincular el programa por medio del ID
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Revisar el programa
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}





int main() {

    EngineFacade ef;

    ef.setWidth(1280);
    ef.setHeight(720);
    ef.setName("XKating");

    char* n = "Link/Link.obj";

    ResourceManager rM;
    TResource* ob = rM.getResource(n);

    //---- Crear la estructura del árbol ----
    TNode *Escena = new TNode();
    //TNode *RotaLuz = new TNode(Escena);
    //TNode *RotaCam = new TNode(Escena);
    TNode *RotaCoche = new TNode(Escena);
    //Escena->addChild (RotaLuz);
    //Escena->addChild (RotaCam);
    Escena->addChild (RotaCoche);
    //TNode *TraslaLuz = new TNode(RotaLuz);
    //TNode *TraslaCam = new TNode(RotaCam);
    TNode *TraslaCoche = new TNode(RotaCoche);
    //RotaLuz->addChild (TraslaLuz);
    //RotaCam->addChild (TraslaCam);
    RotaCoche->addChild (TraslaCoche);

    
    //---- Añadir las entidades a los nodos ----
    //TTransform *TransfRotaLuz = new TTransform ();
    //TTransform *TransfRotaCam = new TTransform ();
    TTransform *TransfRotaCoche = new TTransform ();
    //RotaLuz->setEntity(TransfRotaLuz);
    //RotaCam->setEntity(TransfRotaCam);
    RotaCoche->setEntity(TransfRotaCoche);

    //TTransform *TransfTraslaLuz = new TTransform ();
    //TTransform *TransfTraslaCam = new TTransform ();
    TTransform *TransfTraslaCoche = new TTransform ();
    //TraslaLuz->setEntity(TransfTraslaLuz);
    //TraslaCam->setEntity(TransfTraslaCam);
    TraslaCoche->setEntity(TransfTraslaCoche);

    //TNode *NLuz = new TNode(Escena);
    //TNode *NCam = new TNode(Escena);
    TNode *NChasis = new TNode(Escena);
    //TraslaLuz->addChild (NLuz);
    //TraslaCam->addChild (NCam);
    TraslaCoche->addChild (NChasis);


    //TLight *EntLuz = new TLight();
    //TCamera *EntCam = new TCamera(45);
    TMesh *MallaChasis = new TMesh();
    MallaChasis->setMesh(ob);
            std::cout<<"adios"<<"\n";
    //NLuz->setEntity(EntLuz);
    //NCam->setEntity(EntCam);
    NChasis->setEntity(MallaChasis);


    //=====================================================================================
    // OPENGL TEST
    //=====================================================================================


    sf::ContextSettings settings = sf::ContextSettings(0,0,0,4.5,0);
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "XKating", sf::Style::Default, settings);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Falló al inicializar GLEW\n" << std::endl;
        return -1;
    }
    else {
        std::cout << "GLEW initialized" << std::endl;
    }


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);    


    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("Link/Link.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    GLfloat *vertex, *normals, *textures;
    GLint* vertexIndices;

    aiMesh* mesh = scene->mMeshes[0];

    int nFaces = mesh->mNumFaces;
    int nTriangles = mesh->mNumFaces;
    int nVertex = mesh->mNumVertices;

    vertex = (float *)malloc(sizeof(float) * nVertex * 3);
    memcpy(&vertex[0], mesh->mVertices, 3 * sizeof(float) * nVertex);
    normals = (float *)malloc(sizeof(float) * nVertex * 3);
    memcpy(&normals[0], mesh->mNormals, 3 * sizeof(float) * nVertex);
    textures = (float *)malloc(sizeof(float) * nVertex * 4);
    memcpy(&textures[0], mesh->mTextureCoords, 4 * sizeof(float) * nVertex);

    //We assume we are always working with triangles
    vertexIndices = (GLint *)malloc(sizeof(GLint) * nFaces * 3);
    unsigned int faceIndex = 0;

    for(unsigned int j = 0; j<nFaces; j++)
    {
        memcpy(&vertexIndices[faceIndex], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
        faceIndex += 3;
    }

    //for(unsigned int i = 0; i < nVertex * 3; i++){
    //    std::cout << vertex[i] << std::endl;
    //}
    //for(unsigned int i = 0; i < nVertex * 3; i++){
    //    std::cout << normals[i] << std::endl;
    //}

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    GLuint programID = LoadShaders( "test.vertexshader", "test.fragmentshader" );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    glm::mat4& Projection = Escena->getEntity()->projectionMatrix();
    Projection = glm::perspective(glm::radians(45.0f), (float) 16 / (float)9, 0.1f, 100.0f);

    glm::mat4& View = Escena->getEntity()->viewMatrix();
    View = glm::lookAt( glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0) );

    glm::mat4& Model = Escena->getEntity()->modelMatrix();
    Model = glm::mat4(1.0f);

    GLuint model = glGetUniformLocation(programID, "ModelMatrix");   
    GLuint view  = glGetUniformLocation(programID, "ViewMatrix");
    GLuint projection = glGetUniformLocation(programID, "ProjectionMatrix");

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);


    sf::ContextSettings settingss = window->getSettings();
    std::cout << "version:" << settingss.majorVersion << "." << settingss.minorVersion << std::endl;

    while(window->isOpen())
    {
        glUniformMatrix4fv(model, 1, GL_FALSE, &Escena->getEntity()->modelMatrix()[0][0]);
        glUniformMatrix4fv(view, 1, GL_FALSE, &Escena->getEntity()->viewMatrix()[0][0]);
        glUniformMatrix4fv(projection, 1, GL_FALSE, &Escena->getEntity()->projectionMatrix()[0][0]);


        //////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, nTriangles); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        ///////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->setActive();
        window->display();
    }


    return 0;

}