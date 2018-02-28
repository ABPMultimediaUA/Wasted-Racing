#include "TMesh.h"

void TMesh::beginDraw()
{
    if(mesh)
    {
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        if(textActive)
        {
            texture->draw();
        }
        mesh->draw();
    }
}