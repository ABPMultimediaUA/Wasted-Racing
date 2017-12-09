/*#include "AudioFMOD.h"

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
    ERRCHECK(FMOD::Studio::System::create(&system));
    ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
    ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    //Initialize banks
    ERRCHECK(system->loadBankFile("lib/audio/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(system->loadBankFile("lib/audio/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
    ERRCHECK(system->loadBankFile("lib/audio/Menu.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank));

    //Initialize Event
    ERRCHECK(system->getEvent("event:/Accept", &acceptDescription));
    acceptInstance = new FMOD::Studio::EventInstance*[10];

}

void AudioFMOD::updateAudioEngine() {

}

void AudioFMOD::closeAudioEngine() {
    
}

void AudioFMOD::playObject(int i) {

}

void AudioFMOD::playSound(){

    
    if(i<10)
    {
        ERRCHECK(acceptDescription->createInstance(&acceptInstance[i]));
        ERRCHECK(acceptInstance[i]->start());
        ERRCHECK(acceptInstance[i]->release());
        i++;
    }
    /*
    ERRCHECK(acceptDescription->createInstance(&acceptInstance));
    ERRCHECK(acceptInstance->start());
    ERRCHECK(acceptInstance->release());
    */
/*
    ERRCHECK(system->update());
}
*/