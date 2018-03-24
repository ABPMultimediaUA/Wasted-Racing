#include "AudioFMOD.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/AudioManager.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

//==============================================================================================================================
// MACROS
//==============================================================================================================================

//Error management
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
} 

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

//Macro for importing new Banks ands EventDescriptions
#define LOAD_EVENT(bank, event) \
    if (banks.find(#bank) == banks.end()) { \
        banks.insert(std::pair<std::string, FMOD_STUDIO_BANK*>(#bank, nullptr)); \
        ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/"#bank".bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &banks[#bank])); \
    } \
    if (eventDescriptions.find(#event) == eventDescriptions.end()) { \
        eventDescriptions.insert(std::pair<std::string, FMOD_STUDIO_EVENTDESCRIPTION*>(#event, nullptr)); \
        ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/"#event,  &eventDescriptions[#event])); \
    }
    

//==============================================================================================================================
// DELEGATES DECLARATIONS
//==============================================================================================================================
//COLLISIONS
void shootDefaultCollisionEvent(EventData e);
void shootRampCollisionEvent(EventData e);
void shootItemBoxCollisionEvent(EventData e);
void shootBananaCollisionEvent(EventData e);
//SCORE
void shootOnNewLapEvent(EventData e);
void shootOnOvertakeEvent(EventData e);
void shootOnOvertakenEvent(EventData e);



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

        LOAD_EVENT(CharacterES, CharacterES);

    }
    else{

    }

    //Listeners
    //COLLISION
    EventManager::getInstance().addListener(EventListener {EventType::Default_Collision, shootDefaultCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RampComponent_Collision, shootRampCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision, shootItemBoxCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BananaComponent_Collision, shootBananaCollisionEvent});
    //SCORE
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnNewLap, shootOnNewLapEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnOvertake, shootOnOvertakeEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Score_OnOvertaken, shootOnOvertakenEvent});

    //Game veriables
    worldUnits = 0.05;
    gameVolume = 1.0;

}

//Updater
void AudioFMOD::update() {

    //Update listener position and orientation
    setListenerPosition();

    //Update position of events
    for(auto event : soundEvents) {        
        if(!event.second->isPlaying() || event.second->getEmitter().expired()) {
            //If the event has stopped playing or the object it comes from has been destroyed
            delete event.second;
            soundEvents.erase(event.first);
        }
        else {
            //Set sound position
            auto pos = event.second->getEmitter().lock().get()->getGameObject().getTransformData().position;
            event.second->setPosition(pos * worldUnits);
        }

    }

    //Update FMOD system
    ERRCHECK( FMOD_Studio_System_Update(system) );

}

//Closer
void AudioFMOD::closeAudioEngine() {

    for(auto description : eventDescriptions)
        ERRCHECK( FMOD_Studio_EventDescription_ReleaseAllInstances(description.second) );

    for(auto bank : banks)
        ERRCHECK( FMOD_Studio_Bank_Unload(bank.second) );

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

//Inserts a new event if it doesn't exist
void AudioFMOD::insertSoundEvent(std::string name, ISoundEvent* sound) { 
    soundEvents[name] = sound;
}

//Returns true if we have a soundEvent with that name
bool AudioFMOD::existsSoundEvent(std::string name) {
    if(soundEvents.find(name) == soundEvents.end())
        return false;
    return true;
}

//==============================================================================================================================
// DELEGATE FUNCTIONS
//==============================================================================================================================
//COLLISION
void shootDefaultCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "DefaultCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootRampCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "RampCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootItemBoxCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "ItemBoxCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
void shootBananaCollisionEvent(EventData e) {
    
    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "BananaCollisionEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);
  
}
//SCORE
void shootOnNewLapEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnNewLapEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

}
void shootOnOvertakeEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakeEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

}
void shootOnOvertakenEvent(EventData e) {

    ISoundEvent* sound = ISoundEvent::createSound(ISoundEvent::getFactoryMap(), "OnOvertakenEvent");
    AudioFMOD* audioFMOD = (AudioFMOD*)AudioManager::getInstance().getAudioFacade();
    sound->initalizeSound(audioFMOD, e);

}
