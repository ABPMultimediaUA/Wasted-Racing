#include "TMesh.h"

void TMesh::beginDraw()
{
    if(mesh)
    {
        //glUniformMatrix4fv(model, 1, GL_FALSE, &scene->getEntity()->modelMatrix()[0][0]);
        if(textActive)
        {
            texture->draw();
        }
        mesh->draw();
    }
}