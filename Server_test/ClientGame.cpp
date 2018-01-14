
#include <iostream>

void holita(){
	for(int i = 0; i < 1000000000; ++i){

	}
}

int main(){

	std::cout << "Starting game..." << std::endl;

	//holita();
	
	Game game;
	
	game.Run();
	
	std::cout << "Exiting game..." << std::endl;
	
	return 0;  

}

