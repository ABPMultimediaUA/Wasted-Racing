#include "Game.h"

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdio.h>



//====================================================
//  GAME INITIALIZATION
//====================================================
void Game::init() {

    //Say game loop is active
    setStay(true);

    //Initial state
    setState(&IntroState::getInstance());

    //Set engine to default
    setRenderEngine(0);
    setInputEngine(0);
}

//====================================================
//  GAME UPDATE
//====================================================
void Game::update(float dTime) {


}

//====================================================
//  GAME DRAW
//====================================================
void Game::draw() {

}

//====================================================
//  GAME CLOSE
//====================================================
void Game::close() {
    state->close();
}

//====================================================
//  GAME LOOP
//====================================================
void Game::Run() {
    //Initialize game
    init();
    
    //Initialize timer
    /*auto lastTime = std::chrono::high_resolution_clock::now();
    float accumulatedTime = 0;*/

    //Start the run
    //execute game while staying
    while(stay){
        std::cout<<"A ver, esto  no tiene sentido ninguno: "<< &Game::getInstance() <<std::endl;

        //Measure elapsed time
       /* auto currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currTime - lastTime;
        lastTime = currTime;
        accumulatedTime += (float)elapsed.count();*/
         float accumulatedTime = 0.f;
        //Update the game once every maxTime
        state->update(accumulatedTime);

        //Always draw the game
        state->draw();
    }

    Game::close();
}