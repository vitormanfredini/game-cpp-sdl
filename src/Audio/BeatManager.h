#pragma once

class BeatManager {
public:

    enum class BeatType {
        NoBeat,
        Strong,
        Weak
    };

    struct BeatToProcess {
        BeatType beatType;
        int offsetSamples;
        
        BeatToProcess(BeatType beatType, int offsetSamples)
            :   beatType(beatType),
                offsetSamples(offsetSamples) {}
    };

    BeatManager(int samplesPerBeat)
        :   samplesPerBeat(samplesPerBeat) {
        reset();
    }

    ~BeatManager(){
        //
    }

    BeatToProcess updateAndGetBeatToProcess(int samplesToProcess) {

        if(!playing){
            return BeatToProcess(BeatType::NoBeat, 0);
        }
        
        bool isFirstBeat = measuresCount == 0 && measureBeatsCount == 0 && beatSamplesCount == 0;

        beatSamplesCount += samplesToProcess;

        if(isFirstBeat){
            measureBeatsCount++;
            return BeatToProcess(BeatType::Strong, 0);
        }

        bool hasBeat = beatSamplesCount >= samplesPerBeat;

        if(!hasBeat){
            return BeatToProcess(BeatType::NoBeat, 0);
        }

        beatSamplesCount -= samplesPerBeat;

        BeatType beatType = measureBeatsCount == 0 ? BeatType::Strong : BeatType::Weak;
        int offset = samplesToProcess - beatSamplesCount;

        measureBeatsCount++;
        if(measureBeatsCount == 4){
            measureBeatsCount = 0;
            measuresCount++;
        }

        return BeatToProcess(beatType, offset);

    }

    void play(){
        playing = true;
    }

    void reset(){
        beatSamplesCount = 0;
        measureBeatsCount = 0;
        measuresCount = 0;
        playing = false;
    }

private:

    int samplesPerBeat;
    int beatSamplesCount;
    int measureBeatsCount;
    int measuresCount;

    bool playing;

};
