#pragma once

class TEntity{

public:

    TEntity() {}
    virtual ~TEntity() = 0;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

private:

};