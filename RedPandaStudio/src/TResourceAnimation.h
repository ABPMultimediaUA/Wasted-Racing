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

        //Plays a no-loop animation that is currently paused
        void playNoLoop();

        //Updates the animation
        void update(double eTime);

        //////////////////////////////////////////////////////////////////////
        /////////////////////////// GETTERS && SETTERS ///////////////////////
        //////////////////////////////////////////////////////////////////////
        int getFrames()                 {   return frames;      };
        bool getPauseState()            {   return playing;     };
        bool getLoopState()             {   return loop;        };
        double getFramerate()           {   return framerate;   };
        void setFrames(int i)           {   frames = i;         };
        void setPause(bool b)           {   playing = b;        };
        void setLoop(bool b)            {   loop = b;           };
        void setFramerate(double s)     {   framerate = s;      };

    private:
        //Maximum number of frames (initially set to 60)
        int frames = 60;
        //Pointer to current frame
        int pointer = 0;
        //Framerate of the animation
        double framerate = 1/24;
        //Elapsed time since the previous frame
        double elapsedTime = 0;

        //Array of frames
        std::vector<TResourceOBJ*> objs;

        //Pauses or plays the animation
        bool playing = true;
        //Controls if the animation is played in loop or not
        bool loop = true;

        //Auxiliar function to split strings
        std::vector<std::string> split(const std::string& s, const char& c);
};