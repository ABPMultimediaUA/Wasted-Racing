#pragma once

class TEntity{

public:

    TEntity() {}
    ~TEntity() {}

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

private:

};