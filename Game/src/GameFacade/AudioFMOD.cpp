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
void addDefaultCollision(EventData eData); 

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
        ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/CharacterES.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &characterBank));


        ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/CharacterES", &characterDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(characterDescription, &characterInstance));
    }

    else
        ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/CharacterES.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &characterBank));


    //Listeners

    WorldUnits = 0.05;

    player = 0;
    track = 0;
    change = true;
}

//Updater
void AudioFMOD::update() {

    //Update listener position
    FMOD_3D_ATTRIBUTES attributes;
    auto pos = getListener().getTransformData().position;
    auto vel = getListener().getComponent<MoveComponent>().get()->getMovemententData().velocity;
    auto ang = getListener().getComponent<MoveComponent>().get()->getMovemententData().angle;
    attributes.position = { pos.x * WorldUnits, pos.y * WorldUnits, pos.z * WorldUnits };
    attributes.velocity = { vel.x * WorldUnits, vel.y * WorldUnits, vel.z * WorldUnits };
    attributes.forward = { -std::cos(ang), 0.0f, -std::sin(ang) };
    attributes.up = { 0.0f, -1.0f, 0.0f };

    ERRCHECK( FMOD_Studio_System_SetListenerAttributes(system, 0, &attributes) );

    //=================================================================
    FMOD_STUDIO_PLAYBACK_STATE state;

    FMOD_Studio_EventInstance_GetPlaybackState(characterInstance, &state);

    if(state==FMOD_STUDIO_PLAYBACK_STOPPED)
    {
        
        FMOD_Studio_EventInstance_SetParameterValue(characterInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(characterInstance, "track", (float)track);

        if(change)
            change = false;
        else
            change = true;

        if(change){
            track++;
            if(track>8){
                track=0;
                player++;
            }
        }

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = 76 * WorldUnits;
        attributes.position.y = 0;
        attributes.position.z = 9 * WorldUnits;
        FMOD_Studio_EventInstance_Set3DAttributes(characterInstance, &attributes);

        ERRCHECK( FMOD_Studio_EventInstance_Start(characterInstance) );
    }
    //=================================================================

    //Update FMOD system
    ERRCHECK( FMOD_Studio_System_Update(system) );

}

//Closer
void AudioFMOD::closeAudioEngine() {

    //We should do a release for every class, instance and description

    ERRCHECK( FMOD_Studio_EventDescription_ReleaseAllInstances(characterDescription) );

    ERRCHECK( FMOD_Studio_Bank_Unload(characterBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(stringsBank) );
    ERRCHECK( FMOD_Studio_Bank_Unload(masterBank) );

    ERRCHECK( FMOD_Studio_System_Release(system) );
}

//Sets the basic volume of the game. Expected value between 0 and 1;
void AudioFMOD::setVolume(float vol) {

}

//Sets the 3D position of the listener
void AudioFMOD::setListenerPosition(glm::vec3 pos) {

}

//Creates an audio event instance
void AudioFMOD::createAudioInstance(AudioManager::AudioType type, glm::vec3 pos, std::string parameters) {

}

//==============================================================================================================================
// DELEGATE FUNCTIONS
//==============================================================================================================================
void addDefaultCollision(EventData eData) {

    int player = std::dynamic_pointer_cast<MoveComponent>(eData.Component).get()->getMovemententData().player;
    int track = 5;
    glm::vec3 pos = eData.Component.get()->getGameObject().getTransformData().position;

    AudioManager::getInstance().getAudioFacade()->createAudioInstance(AudioManager::AudioType::Character, pos, "player:" + std::to_string(player) + ",track:" + std::to_string(track));

}