#include "Game.h"

#include <iostream>

#include <fmod_studio.hpp>
#include <fmod.hpp>

void holita(){
/*
	void *extraDriverData = NULL;

    FMOD_STUDIO_SYSTEM* system = NULL;
	FMOD_SYSTEM* lowLevelSystem = NULL;

    ERRCHECK( FMOD_Studio_System_Create(&system, FMOD_VERSION) );
    ERRCHECK( FMOD_Studio_System_GetLowLevelSystem(system, &lowLevelSystem) );
    ERRCHECK( FMOD_System_SetSoftwareFormat(lowLevelSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0) );
	ERRCHECK( FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT) );
    ERRCHECK( FMOD_Studio_System_Initialize(system, 1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );
    

    FMOD_STUDIO_BANK* masterBank = NULL;
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    FMOD_STUDIO_BANK* stringsBank = NULL;
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
    FMOD_STUDIO_BANK* menuBank = NULL;
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Menu.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank));
    FMOD_STUDIO_BANK* cocodrileBank = NULL;
    ERRCHECK(FMOD_Studio_System_LoadBankFile(system, "media/audio/banks/Crocodile.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &cocodrileBank));

    //Initialize Event
    FMOD_STUDIO_EVENTDESCRIPTION* acceptDescription = NULL;
    FMOD_STUDIO_EVENTINSTANCE* acceptInstance = NULL;
    ERRCHECK(FMOD_Studio_System_GetEvent(system, "event:/Accept", &acceptDescription));

        ERRCHECK( FMOD_Studio_EventDescription_LoadSampleData(acceptDescription) );


            FMOD_STUDIO_PLAYBACK_STATE state;
            FMOD_STUDIO_EVENTINSTANCE* eventInstance = NULL;
            ERRCHECK( FMOD_Studio_EventDescription_CreateInstance(acceptDescription,&eventInstance )  );
            FMOD_Studio_EventInstance_GetPlaybackState(eventInstance, &state);
            std::cout << state << std::endl;
            ERRCHECK( FMOD_Studio_EventInstance_Start(eventInstance) );
            FMOD_Studio_EventInstance_GetPlaybackState(eventInstance, &state);
            std::cout << state << std::endl;

            while(true){
                FMOD_Studio_EventInstance_GetPlaybackState(eventInstance, &state);
                std::cout << state << std::endl;
                
                if(state==FMOD_STUDIO_PLAYBACK_STOPPED)
                {
                    ERRCHECK( FMOD_Studio_EventInstance_Start(eventInstance) );
                    //ERRCHECK( FMOD_Studio_EventInstance_Release(eventInstance) );
                }

                ERRCHECK( FMOD_Studio_System_Update(system) );
            }
            */
    /*FMOD_STUDIO_BANK* masterBank = NULL;
    ERRCHECK( FMOD_Studio_System_LoadBankFile(system, "C:/Program Files (x86)/FMOD SoundSystem1/FMOD Studio API Windows/api/studio/examples/media/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    
    FMOD_STUDIO_BANK* stringsBank = NULL;
    ERRCHECK( FMOD_Studio_System_LoadBankFile(system, "C:/Program Files (x86)/FMOD SoundSystem1/FMOD Studio API Windows/api/studio/examples/media/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
    
    FMOD_STUDIO_BANK* ambienceBank = NULL;
    ERRCHECK( FMOD_Studio_System_LoadBankFile(system, "C:/Program Files (x86)/FMOD SoundSystem1/FMOD Studio API Windows/api/studio/examples/media/Surround_Ambience.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );
    
    FMOD_STUDIO_BANK* menuBank = NULL;
    ERRCHECK( FMOD_Studio_System_LoadBankFile(system, "C:/Program Files (x86)/FMOD SoundSystem1/FMOD Studio API Windows/api/studio/examples/media/UI_Menu.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank) );
    
    FMOD_STUDIO_BANK* weaponsBank = NULL;
    ERRCHECK( FMOD_Studio_System_LoadBankFile(system, "C:/Program Files (x86)/FMOD SoundSystem1/FMOD Studio API Windows/api/studio/examples/media/Weapons.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &weaponsBank) );

    // Get the Looping Ambience event
    FMOD_STUDIO_EVENTDESCRIPTION* loopingAmbienceDescription = NULL;
    ERRCHECK( FMOD_Studio_System_GetEvent(system, "event:/Ambience/Country", &loopingAmbienceDescription) );
    
    FMOD_STUDIO_EVENTINSTANCE* loopingAmbienceInstance = NULL;
    ERRCHECK( FMOD_Studio_EventDescription_CreateInstance(loopingAmbienceDescription, &loopingAmbienceInstance) );
    
    // Get the 4 Second Surge event
    FMOD_STUDIO_EVENTDESCRIPTION* cancelDescription = NULL;
    ERRCHECK( FMOD_Studio_System_GetEvent(system, "event:/UI/Cancel", &cancelDescription) );
    
    FMOD_STUDIO_EVENTINSTANCE* cancelInstance = NULL;
    ERRCHECK( FMOD_Studio_EventDescription_CreateInstance(cancelDescription , &cancelInstance) );
    
    // Get the Single Explosion event
    FMOD_STUDIO_EVENTDESCRIPTION* explosionDescription = NULL;
    ERRCHECK( FMOD_Studio_System_GetEvent(system, "event:/Explosions/Single Explosion", &explosionDescription) );

    // Start loading explosion sample data and keep it in memory
    ERRCHECK( FMOD_Studio_EventDescription_LoadSampleData(explosionDescription) );



            FMOD_STUDIO_EVENTINSTANCE* eventInstance = NULL;
            ERRCHECK( FMOD_Studio_EventDescription_CreateInstance(explosionDescription,&eventInstance )  );

            ERRCHECK( FMOD_Studio_EventInstance_Start(eventInstance) );*/



    
}

int main(){

	std::cout << "Starting game..." << std::endl;

	holita();
	
	Game game;
	
	game.Run();
	
	std::cout << "Exiting game..." << std::endl;
	
	return 0;  

}

