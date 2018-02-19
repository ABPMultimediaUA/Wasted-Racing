#version 450 core

precision mediump float;	// Precisión media

varying vec4 v_Color;		// in: color del vertex shader

in vec2 UV_Coordinates; //UV del vertex shader

uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
  FragColor = texture(sampler, UV_Coordinates) * v_Color;
  //FragColor = v_Color;
}