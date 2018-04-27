#version 450 core

layout(location = 0) in vec4 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

varying vec4 v_Color;


void main()
{

    gl_Position = modelViewProjectionMatrix * vertexPosition;

}
