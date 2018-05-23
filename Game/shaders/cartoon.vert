#version 450 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 UV_Coordinates;
out vec3 P;
out vec3 N;
out vec4 CamPos;

out mat4 view;
out mat4 modelViewMatrix;

out mat3 TBN;

//================================
uniform vec4 lightSpaceView;

varying vec4 FragPos;
varying vec4 FragLightPos;
varying vec3 Normal;

out vec4 lightPos;
out vec4 viewPos;
//================================

void main()
{
    modelViewMatrix = ViewMatrix * ModelMatrix;

    CamPos = vec4(modelViewMatrix[3][0], modelViewMatrix[3][1], modelViewMatrix[3][2], modelViewMatrix[3][0]);

    P = vec3(modelViewMatrix * vertexPosition);	          // Posición del vértice
	N = vec3(modelViewMatrix * vec4(vertexNormal, 0.0));    // Normal del vértice

    view = ViewMatrix;

    vec3 T = normalize(vec3(ModelMatrix * vec4(tangents, 0.0)));
    vec3 B = normalize(vec3(ModelMatrix * vec4(bitangents, 0.0)));
    vec3 N = normalize(vec3(ModelMatrix * vec4(vertexNormal, 0.0)));
    TBN = mat3(T, B, N);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vertexPosition;

    UV_Coordinates = UV;
}