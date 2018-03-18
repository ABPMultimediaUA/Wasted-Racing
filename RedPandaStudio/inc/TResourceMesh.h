#pragma once

#include "TEntity.h"
#include "TResource.h"
#include "TResourceMaterial.h"
#include "TResourceTexture.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class TResourceMesh : public TResource {
    public:
        TResourceMesh() {};
        ~TResourceMesh() {};

        //Copies the data needed from an already loaded in memory mesh (loaded with assimp)
        bool loadMesh(aiMesh* m);

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////  GETTERS && SETTERS
        //////////////////////////////////////////////////////////////////////////////////////////

        void setVertex(GLfloat* v)              { vertex=v;         }
        void setNormals(GLfloat* n)             { normals=n;        }
        void setTextures(GLfloat* t)            { textures=t;       }
        void setIndices(GLuint* i)              { vertexIndices=i;  }
        void setNTriangles(long n)              { nTriangles=n;     }
        void setNVertex(long n)                 { nVertex=n;        }
        void setTexture(TResourceTexture* t)    { texture=t;        }
        void setTextActive(bool b)              { textActive=b;     }
        void setMaterial(TResourceMaterial* m)  { material=m;       }

    private:
        //Vertex info
        GLfloat* vertex, *normals, *textures;
        //Vertex indices
        GLuint* vertexIndices;
        //Number of faces (assuming faces are triangles, what we currently are forcing)
        long nTriangles = 0;
        //Number of vertex
        long nVertex;
        //Texture asociated to this mesh
        TResourceTexture* texture = NULL;
        //Control variable to see if the texture is active
        bool textActive=0;
        //Material asociated to this mesh
        TResourceMaterial* material = NULL;
        //Buffer handles
        GLuint* vboHandles;
};