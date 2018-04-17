#pragma once

#include "TResource.h"
#include "TResourceOBJ.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//This class is only intended to load frame by frame animations, defining each frame in it's own OBJ file.
//The purpose of this class is not loading skeletical or any other type of animations

//At the moment, the route provided should direct to the file, except the final three numbers and the .obj extension. For example:
//Animation000001.obj ... Animation000999.obj -> Animation000
//Only supports a maximum of 999 frames
//Warning: This class doesnt check the existence of all the frames, if you try to load more frames than those that exist, you will cause a segmentation fault
class TResourceAnimation : public TResource {
    public:
        TResourceAnimation(){};
        ~TResourceAnimation(){};

        void populateTextures();

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

        //////////////////////////////////////////////////////////////////////
        /////////////////////////// GETTERS && SETTERS ///////////////////////
        //////////////////////////////////////////////////////////////////////
        int getFrames()             {   return frames;  };
        void setFrames(int i)       {   frames = i;     };

    private:
        //Maximum number of frames (initially set to 61)
        int frames = 61;
        //Pointer to current frame
        int pointer = 0;
        //Array of frames
        std::vector<TResourceOBJ*> objs;


        //Auxiliar function to split strings
        std::vector<std::string> split(const std::string& s, const char& c);
};