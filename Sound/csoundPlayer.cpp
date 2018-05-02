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
    float   probability;
    float*** notes;
};

struct duration {
    float currentDuration;
    float cummulatedDuration;
    float trackSilence;
    float trackPlay;
};

//Declare function for generating new notes
void getNextNote(const track& newTrack, uint8_t & lastNote, float & duration);

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
        fread(&newTrack->probability, sizeof(float), 1, pFile);

        std::cout << newTrack->probability << std::endl;

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

    //Output track proabilities
    //for (int i = 0; i < tracks[2]->noteSize; i++) {
    //
    //    for(int j = 0; j < tracks[2]->noteSize; j++) {
    //        
    //            std::cout << tracks[2]->notes[i][j][0] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    std::string orchestra =    "sr = 44100\n\
                                ksmps = 32\n\
                                nchnls = 2\n\
                                0dbfs = 1\n\
                                \n\
                                giengine fluidEngine\n\
                                ; soundfont path to manual/examples\n\
                                isfnum	 fluidLoad \"FluidSynth.sf2\", giengine, 1\n";

    //Load programs to be used
    for(int i = 0; i < size; i++) {
        orchestra += "	 fluidProgramSelect giengine, " + std::to_string(tracks[i]->channel) + ", isfnum, 0, " + std::to_string(tracks[i]->program) + "\n";
    }

    //Load instruments to be used
    for(int i = 0; i < size; i++) {
        orchestra += "	    instr " + std::to_string(i+1) + "\n\
                            \n\
                            	mididefault   60, p3\n\
                            	midinoteonkey p4, 100\n\
                                \n\
                                ichannel   = " + std::to_string(tracks[i]->channel) + " \n\
                                ikey       = p4 \n\
                                ivelocity  = 100 \n\
                                \n\
                                fluidNote giengine, ichannel, ikey, ivelocity \n\
                                \n\
                            endin\n\
                            \n";
    }

    //Load midi instrument to be used
    orchestra +=           "instr 99\n\
                            \n\
                            imvol  init 7\n\
                            asigl, asigr fluidOut giengine\n\
                                   outs asigl*imvol, asigr*imvol\n\
                            \n\
                            endin";

    //Load score
    std::string score = "i 99 0 " + std::to_string(playTime) + " \n";

    //Generate score out of probabilities for playTime time
    std::vector<duration> durations;
    std::vector<uint8_t>  lastNote;

    //Initilize vectors
    for(int i = 0; i < size; i++) {
        durations.push_back(duration{ 0.0, 0.0, 0.0, 0.0});
        lastNote.push_back(tracks[i]->minTone);
    }

    for(float i = 0; i < playTime; i += 0.0625){

        //Update every track
        for(int j = 0; j < size; j++) {
            //If we've got silence, add a note
            if(durations[j].cummulatedDuration <= 0 && durations[j].trackSilence <= 0) {

                float prob = ((int)rand())/((float)INT_MAX);

                if(tracks[j]->probability >= prob || durations[j].trackPlay > 0) {
                    getNextNote(*tracks[j], lastNote[j], durations[j].currentDuration);
                    score += "i " + std::to_string(j+1) + " " + std::to_string(i) + " " + std::to_string(durations[j].currentDuration);
                    score += " " + std::to_string(lastNote[j]) + "\n";

                    durations[j].cummulatedDuration = durations[j].currentDuration;

                    durations[j].trackPlay -= durations[j].currentDuration;
                }
                else {
                    prob = ((int)rand())/((float)INT_MAX);
                    durations[j].trackSilence = 0.5*prob*playTime*(1-tracks[j]->probability);
                    durations[j].trackPlay = 0.5*prob*playTime*tracks[j]->probability;
                }
                
                
            }
            else{
                durations[j].cummulatedDuration -= 0.0625;
                durations[j].trackSilence -= 0.0625;
            }
        }
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

    //Delete all the data we've used
    for(int i = 0; i < tracks.size(); i++) {

        int noteSize = tracks[i]->noteSize;

        for(int j = 0; j < noteSize; j++) {
            for(int k = 0; k < noteSize; k++) {
                delete [] tracks[i]->notes[j][k];
            }
            delete [] tracks[i]->notes[j];
        }
        delete [] tracks[i]->notes;
        delete tracks[i];
    }

    return 0;

}


void getNextNote(const track& newTrack, uint8_t & lastNote, float & duration) {

    float probability = ((int)rand())/((float)INT_MAX);

    float cummulatedProb = 0.0;

    float*** notes = newTrack.notes;

    for(int i = 0; i < newTrack.noteSize; i++) {
        cummulatedProb += notes[lastNote-newTrack.minTone][i][0];

        if(cummulatedProb > probability){
            //New note to be returned
            lastNote = newTrack.minTone + i;

            //Calculate probability duration
            probability = ((int)rand())/((float)INT_MAX);

            cummulatedProb = 0.0;
            for(int j = 1; j < 9; j++) {

                cummulatedProb += notes[lastNote-newTrack.minTone][i][j];

                if(cummulatedProb > probability){
                    
                    duration = pow(2, j) * 0.03125;

                    return;
                }
            }
        }
            

    }

}