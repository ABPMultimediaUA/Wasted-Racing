#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <stack>
#include <iostream>

class TEntity{

public:

    TEntity() {}
    virtual ~TEntity() {};

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

//Those variables are protected to allow children access. If we want to hide them, we should make them private
//and use getters&setters for it's management

    //Model Matrix
    static glm::mat4& modelMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //View Matrix
    static glm::mat4& viewMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //Projection Matrix
    static glm::mat4& projectionMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //Matrix Stack
    static std::stack<glm::mat4>& matrixStack() {
        static std::stack<glm::mat4> matrix;
        return matrix;
    };

    //Setters for initilization
    static void setProgramID(GLuint p)     {   programID = p;  }
    static void setModelID(GLuint m)       {   model = m;      }
    static void setViewID(GLuint v)        {   view = v;       }
    static void setProjectionID(GLuint p)  {   projection = p; }

    //Getters
    static GLuint getProgramID()       {   return programID;   }
    static GLuint getModelID()         {   return model;       }
    static GLuint getViewID()          {   return view;        }
    static GLuint getProjectionID()    {   return projection;  }

protected:

    //Global variables for OpenGL
    static GLuint programID;
    static GLuint model;
    static GLuint view;
    static GLuint projection;

};