#pragma once

#include "TResource.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SFML/Graphics.hpp>


class TResourceMesh : public TResource {
    public:
        TResourceMesh() {};
        ~TResourceMesh() {};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

    private:
        //Vertex info
        GLfloat* vertex, *normals, *textures;
        //Vertex indices
        GLuint* vertexIndices;
        //Number of faces (assuming faces are triangles, what we currently are forcing)
        long nTriangles;
        //Number of vertex
        long nVertex;


        sf::Image image;
        int sizeX, sizeY;


};