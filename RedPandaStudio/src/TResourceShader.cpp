#include "TResourceShader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

bool TResourceShader::loadResource()
{
    //We need to specify the type of shader we want to create
    if(vertexShader)
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    else
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);


    //Read the shader from a file
	std::string shader;
	std::ifstream shaderStream(name, std::ios::in);
	if(shaderStream.is_open()){
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shader = sstr.str();
		shaderStream.close();
	}else{
		printf("Couldn't open %s\n", name);
	}

    //Compile shader
	printf("Compiling shader : %s\n", name);
	char const * shaderString = shader.c_str();
	glShaderSource(shaderID, 1, &shaderString , NULL);
	glCompileShader(shaderID);

    GLint result = GL_FALSE;
	int status;

	//Check there aren't compilation errors
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &status);
	if ( status > 0 ){
		std::vector<char> errorMessage(status+1);
		glGetShaderInfoLog(shaderID, status, NULL, &errorMessage[0]);
		printf("%s\n", &errorMessage[0]);
	}

    return true;
}


void TResourceShader::draw ()
{
    
}