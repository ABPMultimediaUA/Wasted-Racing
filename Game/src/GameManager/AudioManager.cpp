#include "AudioManager.h"
#include "../GameFacade/AudioFMOD.h"

void AudioManager::init() {
    audioFacade = new AudioFMOD();
    audioFacade->openAudioEngine();
}

AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::update() {
    audioFacade->update();
}

void AudioManager::play() {

}

void AudioManager::close() {

}