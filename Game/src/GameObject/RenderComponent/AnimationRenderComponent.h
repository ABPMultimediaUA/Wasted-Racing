#pragma once

#include <string>
#include "IRenderComponent.h"

class AnimationRenderComponent : public IRenderComponent {

public:

    //Constructor
	AnimationRenderComponent(GameObject& newGameObject, const char* newStr, int newFrames, bool newLoop) : 
        IRenderComponent(newGameObject), frames(newFrames), looping(newLoop) {

		std::string folder = "";
		int i = 0;

		while(newStr[i]!='.'){
			folder += newStr[i];
			i++;
		}

		path = "media/anim/" + folder + "/" + std::string(newStr);

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

    //Loop animation
    void loop();

    //Play animation once
    void playOnce();

    //Start animation 
    void play();

	std::string getPath() {
		return path;
	}


private:

	std::string path;
    int frames;
    bool looping;

};