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
void activateA(EventData eData);
void activateK(EventData eData);

void AudioFMOD::openAudioEngine() {
    //Initialize FMOD System
    ERRCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
    ERRCHECK(FMOD_Studio_System_GetLowLevelSystem(system, &lowLevelSystem));
    ERRCHECK(FMOD_System_SetSoftwareFormat(lowLevelSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    //Initialize banks
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Menu.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Crocodile.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &cocodrileBank));

    //Initialize Event
    ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/Accept", &acceptDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(acceptDescription, &acceptInstance));

    acceptEvent = new AcceptEvent(acceptInstance);

    ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/CrocodileGoodEN", &cocodrileGoodENDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(cocodrileGoodENDescription, &cocodrileGoodENInstance));

    cocodrileGoodENEvent = new CocodrileGoodENEvent(cocodrileGoodENInstance);

    EventManager::getInstance().addListener(EventListener {EventType::Key_Flanger_Down, FlangerDown});
    EventManager::getInstance().addListener(EventListener {EventType::Key_Decflanger_Down, FlangerUp});
    EventManager::getInstance().addListener(EventListener {EventType::Key_ActivateA_Down, activateA});
    EventManager::getInstance().addListener(EventListener {EventType::Key_ActivateK_Down, activateK});

    acceptW = true;
    crocodileW = true;

}

void AudioFMOD::update() {
    ERRCHECK(FMOD_Studio_System_Update(system));
}

void AudioFMOD::closeAudioEngine() {
    
}

void AudioFMOD::playSound(){

    if(!acceptEvent->isPlaying() && acceptW)
    {
        acceptEvent->start();
    }
    if(!cocodrileGoodENEvent->isPlaying() && crocodileW)
    {
        cocodrileGoodENEvent->start();
    }
}

void AudioFMOD::IncreaseFlanger()
{
    acceptEvent->increaseFlanger();
}

void AudioFMOD::DecreaseFlanger()
{
    acceptEvent->decreaseFlanger();
}

void AudioFMOD::stopA()
{
    acceptW = !acceptW;
}

void AudioFMOD::stopK()
{
    crocodileW = !crocodileW;
}

void FlangerDown(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->IncreaseFlanger();
}

void FlangerUp(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->DecreaseFlanger();
}

void activateA(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->stopA();
}

void activateK(EventData eData)
{
    AudioManager::getInstance().getAudioFacade()->stopK();
}