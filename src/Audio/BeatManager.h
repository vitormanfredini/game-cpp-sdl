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

    std::vector<BeatUpdateAndOffset> updateAndGetBeatsUpdatesAndOffsets(int samplesToAdd, int correctionDelayInSamples = 0) {

        std::vector<BeatUpdateAndOffset> beatsOffsets = {};

        if(!playing){
            return beatsOffsets;
        }

        int offset = samplesToAdd - beatSamplesCount;

        while(offset < samplesToAdd && offset >= 0){
            beatsOffsets.emplace_back(nextBeat, offset);
            beatSamplesCount -= samplesPerBeat;
            offset += samplesPerBeat;
            nextBeat += 1;
        }

        beatSamplesCount += samplesToAdd - correctionDelayInSamples;

        return beatsOffsets;

    }

    int samplesUntilBeat(int targetBeat){
        int deltaBeats = targetBeat - nextBeat;
        return samplesPerBeat - beatSamplesCount + (deltaBeats * samplesPerBeat);
    }

    void play(){
        playing = true;
    }

    void reset(){
        nextBeat = 0;
        beatSamplesCount = 0;
        playing = false;
    }

private:
    int samplesPerBeat;
    int beatSamplesCount;
    int nextBeat = 0;
    bool playing;

};
