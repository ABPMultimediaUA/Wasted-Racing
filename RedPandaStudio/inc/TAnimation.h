#pragma once

#include "TEntity.h"
#include "TResourceAnimation.h"

class TAnimation : public TEntity{

private:
    TResourceAnimation* animation;

public:

    TAnimation() {}
    ~TAnimation() {}

    //Draw Methods
    void beginDraw();
    void endDraw() {}

    //////////////////////////////////////////////////////////////////////
    ////////////////////// GETTERS & SETTERS /////////////////////////////
    //////////////////////////////////////////////////////////////////////

    TResourceAnimation* getAnimation()                      {   return animation;               }
    void setAnimation(TResourceAnimation* a)                {   animation = a;                  }

};