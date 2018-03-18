#version 450 core

out vec3 tc;

uniform mat4 viewMatrix;

void main(void)
{
    vec3[4] vertices = vec3[4](vec3(-1.0, -1.0, 1.0),
                               vec3( 1.0, -1.0, 1.0),
                               vec3(-1.0,  1.0, 1.0),
                               vec3( 1.0,  1.0, 1.0));

    tc = mat3(viewMatrix) * vertices[gl_VertexID];

    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}
