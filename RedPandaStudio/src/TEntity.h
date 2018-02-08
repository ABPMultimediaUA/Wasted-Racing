#pragma once

#include <glm/ext.hpp>
#include <stack>

class TEntity{

public:

    TEntity() {}
    virtual ~TEntity() {};

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

//Those variables are protected to allow children access. If we want to hide them, we should make them private
//and use getters&setters for it's management
protected:

    //Model Matrix
    static glm::mat4& getModelMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //View Matrix
    static glm::mat4& getViewMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //Projection Matrix
    static glm::mat4& getProjectionMatrix() {
        static glm::mat4 matrix;
        return matrix;
    };

    //Matrix Stack
    static std::stack<glm::mat4>& getMatrixStack() {
        static std::stack<glm::mat4> matrix;
        return matrix;
    };
};