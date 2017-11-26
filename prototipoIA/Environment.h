#include <irrlicht.h>
#include <glm/ext.hpp>

class Environment{
    private:
    irr::scene::ISceneNode** list;
    int size;

    public:
    //Constructor
    Environment();

    //Destructor
    ~Environment();

    //Getters
    irr::scene::ISceneNode** getList();
    int getSize();

    void addNode(irr::scene::ISceneNode* n);
};