#version 450 core
//layout (location = 0) in vec3 vertexPosition;

uniform mat4 lightView;
uniform mat4 lightProjectionMatrix;
uniform mat4 lightModel;

/*varying vec4 color;

void main()
{   
    color = vec4(0.2);

    //gl_Position = lightProjectionMatrix * lightView * lightModel * vec4(vertexPosition, 1.0);
    gl_Position = vec4(vertexPosition, 1.0);
}
*/
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    //=====================================================


    //=====================================================
    //tc = mat3(ViewMatrix) * vertices[gl_VertexID];
    /*vec3[4] vertices = vec3[4](vec3(-1.0, -1.0, 1.0),
                               vec3( 1.0, -1.0, 1.0),
                               vec3(-1.0,  1.0, 1.0),
                               vec3( 1.0,  1.0, 1.0));
    gl_Position = vec4(vertices[gl_VertexID], 1.0);*/
    //=====================================================
}  