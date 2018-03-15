#version 450 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec4 v_Color;
varying vec2 UV_Coordinates;

const int maxLights = 5;
uniform int numLights;

struct Light {
   vec4 position;
   vec4 intensity;
};
uniform Light light[maxLights];

struct Material {
    vec4 kd;
    vec4 ka;
    vec4 ks;
};
uniform Material material;


void main()
{

    float ambient = 0.6;                               

    v_Color = vec4(0.0, 0.0, 0.0, 1.0);

    vec3 P = vec3(ViewMatrix * ModelMatrix * vertexPosition);	          // Posición del vértice
	vec3 N = vec3(ViewMatrix * ModelMatrix * vec4(vertexNormal, 0.0));    // Normal del vértice

    for(int i = 0; i < numLights && i < maxLights; i++){

        float diffuse = 0.0;

        vec4 LightPos = ViewMatrix * light[i].position;

	    float d = length(LightPos.xyz - P);			        // distancia de la luz
	    vec3  L = normalize(LightPos.xyz - P);			    // Vector Luz

        diffuse = max(dot(N, L), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 80.0/(0.25+(0.1*d)+(0.03*d*d));
        diffuse = diffuse * attenuation;

        v_Color += vec4(light[i].intensity * diffuse) * material.kd;

    }

    v_Color += vec4(1.0, 1.0, 1.0, 1.0) * (ambient);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vertexPosition;

    UV_Coordinates = UV;

}