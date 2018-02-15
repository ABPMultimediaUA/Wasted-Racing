#pragma once

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
};