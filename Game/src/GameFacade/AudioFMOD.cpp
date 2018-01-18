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

    //Listeners

}

void AudioFMOD::update() {

    ERRCHECK( FMOD_Studio_System_Update(system) );

}

void AudioFMOD::closeAudioEngine() {
    
}