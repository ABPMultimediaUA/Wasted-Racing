#include <MidiFile.h>
#include <Options.h>
#include <iostream>
#include <iomanip>
#include <vector>

struct note {
    uint8_t tone;
    uint8_t duration;
};

struct track {
    uint8_t channel;
    uint8_t program;
    uint8_t maxTone = 0;
    uint8_t minTone = 255;
    std::vector<note> notes;
};

struct noteProbability {
    float note;
    uint8_t duration[8] = {0,0,0,0,0,0,0,0};
    unsigned int count = 1;
};

struct notesProbability {
    noteProbability* notes;
    unsigned int count = 0;
};

struct trackProbability {
    uint8_t channel;
    uint8_t program;
    uint8_t maxTone;
    uint8_t minTone;
    notesProbability* notes;
    int size;
};

int main(int argc, char** argv) {

    if(argc < 2){
        std::cerr << "Error: missing arguments" << std::endl;
        return 0;
    }
    
    const char* m = argv[1];
    
    //Open and read midi file
    smf::MidiFile midifile;
    midifile.read(m);
    
    std::cout << "Reading " << argv[1] << "..." << std::endl;
    //Prepare note duration to be read in seconds
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();

    std::vector<track*> tracks;

    //===================================================================================
    //  PARSING MIDI
    //===================================================================================
    int trackCount = midifile.getTrackCount();
    //For every track in the midi, read them
    for (int i=0; i<trackCount; i++) {

        track* newTrack = nullptr;

        for (int event=0; event<midifile[i].size(); event++) {

            //If the track is empty and we receive a valid message
            if(newTrack == nullptr && (midifile[i][event].isNoteOn() || midifile[i][event].isTimbre())){
                newTrack = new track;
            }

            //Get channel and program of the track
            if(midifile[i][event].isTimbre()) {
                newTrack->program = midifile[i][event][1];
                newTrack->channel = midifile[i][event].getChannelNibble();
            }

            //Add note to the track
            if(midifile[i][event].isNoteOn() && midifile[i][event].getDurationInSeconds() > 0) {
                double dur = midifile[i][event].getDurationInSeconds(); 
                
                if(dur > 8)
                    dur = 8.0;
                else if (dur < 0.03125)
                    dur = 0.032;

                //Convert the duration value to a number from 0 to 255, every unit being 0.03125 seconds
                uint8_t duration = ((((int)dur)*8)) + (int)((((float)dur)-((int)dur))*32);
                uint8_t tone     = midifile[i][event][1];
                newTrack->notes.push_back(note {tone, duration});

                //Update min and max tone
                if(newTrack->minTone > tone)
                    newTrack->minTone = tone;
                if(newTrack->maxTone < tone)
                    newTrack->maxTone = tone;
            }    
        }

        if(newTrack != nullptr && newTrack->notes.size()>0) {
            //If track is not empty, add track
            tracks.push_back(newTrack);
        }
    }

    //===================================================================================
    //  CALCULATING PROBABILITIES
    //===================================================================================
    std::cout << "Calculating probabilities..." << std::endl;

    std::vector<noteProbability> probabilities;

    for(int i = 0; i < tracks.size(); i++) {  //for every track

        trackProbability prob = {tracks[i]->channel, tracks[i]->program, tracks[i]->maxTone, tracks[i]->minTone, nullptr, tracks[i]->maxTone - tracks[i]->minTone + 1};

        //initialize probabilities
        prob.notes = new notesProbability[prob.size];

        for(int j = 0; j < prob.size; j++) {    //create every note counter
            prob.notes[j].notes = new noteProbability[prob.size];
        }

    }
    

    return 0;
}
