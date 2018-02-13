#include "TMesh.h"

void TMesh::beginDraw()
{
    if(mesh)
    {
        mesh->draw();
    }
}