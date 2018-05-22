#include "TMesh.h"

void TMesh::beginDraw()
{
    if(mesh)
    {
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        glm::mat4 m = viewMatrix() * modelMatrix();
        glUniformMatrix4fv(TEntity::modelViewID, 1, GL_FALSE, &m[0][0]);
        m = projectionMatrix() * m;
        glUniformMatrix4fv(TEntity::mvpID, 1, GL_FALSE, &m[0][0]);
        mesh->draw();
    }
}