#include "AudioFMOD.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/AudioManager.h"


//==============================================================================================================================
// ERROR MANAGEMENT
//==============================================================================================================================
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

//==============================================================================================================================
// DELEGATES DECLARATIONS
//==============================================================================================================================


//==============================================================================================================================
// AUDIO FMOD FUNCTIONS
//==============================================================================================================================

//Initializer
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

    //Listeners

}

//Updater
void AudioFMOD::update() {

    //Update listener position
    FMOD_3D_ATTRIBUTES attributes;
    auto pos = getListener().getTransformData().position;
    attributes.position.x = pos.x;
    attributes.position.y = pos.y;
    attributes.position.z = pos.z;

    
    std::cout << pos.x << " " << pos.z << std::endl;

    ERRCHECK( FMOD_Studio_System_SetListenerAttributes(system, 0, &attributes) );

    //Update FMOD system
    ERRCHECK( FMOD_Studio_System_Update(system) );

}

//Closer
void AudioFMOD::closeAudioEngine() {

    ERRCHECK( FMOD_Studio_Bank_Unload(menuBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(stringsBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(masterBank) );

    ERRCHECK( FMOD_System_Release(lowLevelSystem ));
    ERRCHECK( FMOD_Studio_System_Release(system) );
}

//Sets the basic volume of the game. Expected value between 0 and 1;
void AudioFMOD::setVolume(float vol) {

}

//Sets the 3D position of the listener
void AudioFMOD::setListenerPosition(glm::vec3 pos) {

}

//==============================================================================================================================
// DELEGATE FUNCTIONS
//==============================================================================================================================