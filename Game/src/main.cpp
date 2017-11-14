#include "Game.h"

#include <iostream>

#include <irrlicht.h>

int main(){

	std::cout << "Starting game..." << std::endl;

	Game game;

	game.Run();

	std::cout << "Exiting game..." << std::endl;

	return 0;  

}