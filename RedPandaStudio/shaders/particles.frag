#version 450 core

varying vec4 v_Color;		// in: color del vertex shader

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}