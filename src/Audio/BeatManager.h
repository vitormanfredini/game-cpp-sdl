#pragma once

class BeatManager {
public:

    BeatManager(int samplesPerBeat)
        :   samplesPerBeat(samplesPerBeat) {
        reset();
    }

    ~BeatManager(){
        //
    }

    std::vector<int> updateAndGetBeatsOffsets(int samplesToAdd) {

        std::vector<int> beatsOffsets = {};

        if(!playing){
            return beatsOffsets;
        }

        int offset = samplesToAdd - beatSamplesCount;
        while(offset < samplesToAdd && offset >= 0){
            beatsOffsets.push_back(offset);
            beatSamplesCount -= samplesPerBeat;
            offset += samplesPerBeat;
        }

        beatSamplesCount += samplesToAdd;

        return beatsOffsets;

    }

    void play(){
        playing = true;
    }

    void reset(){
        beatSamplesCount = 0;
        playing = false;
    }

private:
    int samplesPerBeat;
    int beatSamplesCount;
    bool playing;

};
