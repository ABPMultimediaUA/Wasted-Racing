#include "AudioFMOD.h"
#include "../GameEvent/EventManager.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"


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
void AudioFMOD::openAudioEngine(int lang) {
    //Initialize FMOD System
    ERRCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
    ERRCHECK(FMOD_Studio_System_GetLowLevelSystem(system, &lowLevelSystem));
    ERRCHECK(FMOD_System_SetSoftwareFormat(lowLevelSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    //Initialize banks
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

    if(lang == 1){

    }
    else{

    }


    //Listeners


    //Game veriables
    worldUnits = 0.05;
    gameVolume = 1.0;

}

//Updater
void AudioFMOD::update() {

    //Update listener position and orientation
    setListenerPosition();

    //##############################################################################
    // UPDATEAR AQUI EVENTOS Y TAL
    //##############################################################################

    //Update FMOD system
    ERRCHECK( FMOD_Studio_System_Update(system) );

}

//Closer
void AudioFMOD::closeAudioEngine() {

    //We should do a release for every class, instance and description

    //##############################################################################
    // HACER UN CLOSER PARA CADA COSA EN LOS MAPAS ETC
    //##############################################################################

    ERRCHECK( FMOD_Studio_Bank_Unload(stringsBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(masterBank) );

    ERRCHECK( FMOD_Studio_System_Release(system) );
}

//Sets the basic volume of the game. Expected value between 0 and 1;
void AudioFMOD::setVolume(float vol) {
    gameVolume = vol;
}

//Sets the 3D position of the listener
void AudioFMOD::setListenerPosition() {

    //Update listener position
    FMOD_3D_ATTRIBUTES attributes;
    auto pos = getListener().getTransformData().position;
    auto vel = getListener().getComponent<MoveComponent>().get()->getMovemententData().velocity;
    auto ang = getListener().getComponent<MoveComponent>().get()->getMovemententData().angle;
    attributes.position = { pos.x * worldUnits, pos.y * worldUnits, pos.z * worldUnits };
    attributes.velocity = { vel.x * worldUnits, vel.y * worldUnits, vel.z * worldUnits };
    attributes.forward = { -std::cos(ang), 0.0f, -std::sin(ang) };
    attributes.up = { 0.0f, -1.0f, 0.0f };

    ERRCHECK( FMOD_Studio_System_SetListenerAttributes(system, 0, &attributes) );

}

//==============================================================================================================================
// DELEGATE FUNCTIONS
//==============================================================================================================================
