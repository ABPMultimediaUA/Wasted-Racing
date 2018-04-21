#include "TAnimation.h"

void TAnimation::beginDraw()
{
    if(animation)
    {
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        animation->draw();
    }
}