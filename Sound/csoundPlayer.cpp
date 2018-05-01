#include <iostream>
#include <csound/csound.hpp>
#include <string>
#include <vector>
#include <csound/csPerfThread.hpp>
#include <stdlib.h>  
#include <time.h>  
#include <stdio.h>
#include <climits>

struct track {
    uint8_t channel;
    uint8_t program;
    uint8_t maxTone;
    uint8_t minTone;
    uint8_t noteSize;
    float*** notes;
};

//Declare function for generating new notes
uint8_t getNextNote(const track& newTrack, uint8_t lastNote);

int main(int argc, char** argv) {

    if(argc < 3) {
        std::cout << "First parameter must be the binary file generated from midiPaser" << std::endl;
        std::cout << "Second parameter must be the desired execution time in seconds" << std::endl;
        std::cout << "Example: csoundPlayer probabilities.bin 100" << std::endl;
        std::cout << "         Generate sound for 100 seconds given the probability file" << std::endl;
        return 0;
    }

    srand (time(NULL));

    int playTime = std::atoi(argv[2]);
    const char* binaryFile = argv[1];

    std::cout << "File to read: " << binaryFile << std::endl;
    std::cout << "Playing for: " << playTime << std::endl;

    FILE* pFile;
    pFile = fopen(binaryFile, "rb");

    //Tracks
    std::vector<track*> tracks;
    
    //Write number of tracks
    int size;
    fread(&size, sizeof(int), 1, pFile);

    for (int i = 0; i < size; i++){

        track* newTrack = new track;
        
        //Read per track header
        fread(&newTrack->channel, sizeof(int), 1, pFile);
        fread(&newTrack->program, sizeof(int), 1, pFile);
        fread(&newTrack->maxTone, sizeof(int), 1, pFile);
        fread(&newTrack->minTone, sizeof(int), 1, pFile);
        fread(&newTrack->noteSize, sizeof(int), 1, pFile);

        //Intialize probability array
        newTrack->notes = new float**[newTrack->noteSize];
        for(int j = 0; j < newTrack->noteSize; j++) {
            newTrack->notes[j] = new float*[newTrack->noteSize];
            for(int k = 0; k < newTrack->noteSize; k++) {
                newTrack->notes[j][k] = new float[9];
            }
        }

        for(int j = 0; j < newTrack->noteSize; j++)
            for(int k = 0; k < newTrack->noteSize; k++)
                fread(newTrack->notes[j][k], sizeof(float), 9, pFile);

        tracks.push_back(newTrack);
    }
    fclose(pFile);

    for (int i = 0; i < tracks[2]->noteSize; i++) {
//
        for(int j = 0; j < tracks[2]->noteSize; j++) {
            
                std::cout << tracks[2]->notes[i][j][0] << " ";
        }
        std::cout << std::endl;
    }


std::string orchestra =    "sr = 44100\n\
                            ksmps = 32\n\
                            nchnls = 2\n\
                            0dbfs = 1\n\
                            \n\
                            giengine fluidEngine\n\
                            ; soundfont path to manual/examples\n\
                            isfnum	 fluidLoad \"FluidSynth.sf2\", giengine, 1\n\
                            	 fluidProgramSelect giengine, 9, isfnum, 0, 39\n\
                            \n\
                            instr 1\n\
                            \n\
                            	mididefault   60, p3\n\
                            	midinoteonkey p4, p5\n\
                            ikey	init p4\n\
                            ivel	init p5\n\
                            	fluidNote giengine, 9, ikey, ivel\n\
                            endin\n\
                            \n\
                            instr 99\n\
                            \n\
                            imvol  init 7\n\
                            asigl, asigr fluidOut giengine\n\
                                   outs asigl*imvol, asigr*imvol\n\
                            \n\
                            endin";

std::string score = "i 99 0 60 \n";

uint8_t lastNote = tracks[2]->minTone;
uint8_t lastNote1 = tracks[2]->minTone;
uint8_t lastNote2 = tracks[2]->minTone;
bool hey = false;

for(float i=0; i <200; i += 0.5){
    score += "i 1 " + std::to_string(i) + " 1 " + std::to_string(lastNote) + " 100\n";
    if(hey){
        hey = false;
        score += "i 1 " + std::to_string(i) + " 2 " + std::to_string(lastNote1-12) + " 100\n";
        if(((int)i)%2 == 0){
            score += "i 1 " + std::to_string(i) + " 4 " + std::to_string(lastNote1-24) + " 50\n";
        }
    }
    else {
        hey = true;
    }
    
    lastNote = getNextNote(*tracks[2], lastNote);
    lastNote1 = getNextNote(*tracks[2], lastNote1);
    lastNote2 = getNextNote(*tracks[2], lastNote2);
}

//Create Csound instance
Csound* csound = new Csound();

//Set sound output
csound->SetOption("-odac");

//Compile Orchestra
csound->CompileOrc(orchestra.c_str());

//Read Score
csound->ReadScore(score.c_str());
//prepare Csound for performance
csound->Start();

//set up CsoundPerfThread object 
CsoundPerformanceThread* perfThread = new CsoundPerformanceThread(csound); 

//start Csound performance
perfThread->Play();

//keep the application running while performance is ongoing
while(perfThread->GetStatus() == 0);

//free Csound and thread objects
delete csound;
delete perfThread;
return 0;

}


uint8_t getNextNote(const track& newTrack, uint8_t lastNote) {

    float probability = ((int)rand())/((float)INT_MAX);

    float cummulatedProb = 0.0;

    float*** notes = newTrack.notes;

    for(int i = 0; i < newTrack.noteSize; i++) {
        cummulatedProb += notes[lastNote-newTrack.minTone][i][0];

        if(cummulatedProb > probability)
            return newTrack.minTone + i;

    }

}