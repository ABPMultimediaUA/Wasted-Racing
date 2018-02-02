#pragma once

class TEntity{

    private:

    public:

        TEntity() {}
        ~TEntity() {}

        virtual void beginDraw() = 0;
        virtual void endDraw() = 0;

}