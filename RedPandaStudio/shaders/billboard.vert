#version 450 core

layout (location = 0) in vec3 VPosition;

void main()
{
    gl_Position = vec4(VPosition, 1.0);
}