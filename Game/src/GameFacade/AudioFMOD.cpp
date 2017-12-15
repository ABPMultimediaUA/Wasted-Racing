#include "AudioFMOD.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/AudioManager.h"

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void FlangerDown(EventData eData);
void FlangerUp(EventData eData);

void AudioFMOD::openAudioEngine() {
    //Initialize FMOD System
    ERRCHECK(FMOD::Studio::System::create(&system));
    ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
    ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    //Initialize banks
    ERRCHECK(system->loadBankFile("lib/audio/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(system->loadBankFile("lib/audio/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
    ERRCHECK(system->loadBankFile("lib/audio/Menu.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank));
    ERRCHECK(system->loadBankFile("lib/audio/Crocodile.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &cocodrileBank));

    //Initialize Event
    ERRCHECK(system->getEvent("event:/Accept", &acceptDescription));
    ERRCHECK(acceptDescription->createInstance(&acceptInstance));

    acceptEvent = new AcceptEvent(acceptInstance);

    ERRCHECK(system->getEvent("event:/CrocodileGoodEN", &cocodrileGoodENDescription));
    ERRCHECK(cocodrileGoodENDescription->createInstance(&cocodrileGoodENInstance));

    cocodrileGoodENEvent = new CocodrileGoodENEvent(cocodrileGoodENInstance);

    EventManager::getInstance().addListener(EventListener {EventType::Key_Flanger_Down, FlangerDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Decflanger_Down, FlangerUp});

}

void AudioFMOD::update() {
    ERRCHECK(system->update());
}

void AudioFMOD::closeAudioEngine() {
    
}

void AudioFMOD::playSound(){

    if(!acceptEvent->isPlaying())
    {
        acceptEvent->start();
    }
    if(!cocodrileGoodENEvent->isPlaying())
    {
        cocodrileGoodENEvent->start();
    }
/*
    if(acceptInstance!=NULL)
    {
        
    }
    FMOD_STUDIO_PLAYBACK_STATE state;
    acceptInstance->getPlaybackState(&state);
    if(state!=FMOD_STUDIO_PLAYBACK_PLAYING)
    {
        ERRCHECK(acceptInstance->start());
        ERRCHECK(acceptInstance->release());
    }
    */
}

void AudioFMOD::IncreaseFlanger()
{
    acceptEvent->increaseFlanger();
}

void AudioFMOD::DecreaseFlanger()
{
    acceptEvent->decreaseFlanger();
}

void FlangerDown(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->IncreaseFlanger();
}

void FlangerUp(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->DecreaseFlanger();
}