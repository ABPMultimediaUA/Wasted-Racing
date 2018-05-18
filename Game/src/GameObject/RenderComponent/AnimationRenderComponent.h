#pragma once

#include <string>
#include "IRenderComponent.h"
#include <iostream>
class AnimationRenderComponent : public IRenderComponent {

public:

    //Constructor
	AnimationRenderComponent(GameObject& newGameObject, const char* newStr, int newFrames) : 
        IRenderComponent(newGameObject), frames(newFrames) {

		path = "media/anim/" + std::string(newStr);

	}

	//Destructor
	virtual ~AnimationRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

    //Pause animation
    void pause();

    //Play animation once
    void playOnce();

    //Start animation 
    void playLoop();

	std::string getPath() {
		return path;
	}

	int getState() 		{ return state; }
	int getFrames()		{ return frames; }

private:

	std::string path;
    int frames;
    int state = 2; // 0 = paused, 1 = play once, 2 = play loop

};