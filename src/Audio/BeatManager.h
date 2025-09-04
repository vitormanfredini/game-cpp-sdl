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

    struct BeatUpdateAndOffset {
        int beat;
        int offset;

        BeatUpdateAndOffset(int beat, int offset): beat(beat), offset(offset) {}
    };

    std::vector<BeatUpdateAndOffset> updateAndGetBeatsUpdatesAndOffsets(int samplesToAdd) {

        std::vector<BeatUpdateAndOffset> beatsOffsets = {};

        if(!playing){
            return beatsOffsets;
        }

        int offset = samplesToAdd - beatSamplesCount;
        while(offset < samplesToAdd && offset >= 0){
            beatsOffsets.emplace_back(beats, offset);
            beatSamplesCount -= samplesPerBeat;
            offset += samplesPerBeat;
            beats += 1;
        }

        beatSamplesCount += samplesToAdd;

        return beatsOffsets;

    }

    void play(){
        playing = true;
    }

    void reset(){
        beats = 0;
        beatSamplesCount = 0;
        playing = false;
    }

private:
    int samplesPerBeat;
    int beatSamplesCount;
    int beats = 0;
    bool playing;


};
