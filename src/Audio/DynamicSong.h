#pragma once

#include <vector>
#include <iostream>

class DynamicSong {
public:

    struct Section {
        int mainSoundId;
        int fillSoundId;
        int variationSoundId;
        int outroSoundId;
        int beat; // 0,1,2,3
        
        Section(
            int mainSoundId,
            int fillSoundId,
            int variationSoundId,
            int outroSoundId
        ):
            mainSoundId(mainSoundId),
            fillSoundId(fillSoundId),
            variationSoundId(variationSoundId),
            outroSoundId(outroSoundId),
            beat(0) {
                //
            }
    };

    DynamicSong(){
        //
    }

    ~DynamicSong(){
        //
    }

    void addSection(
        int mainSoundId,
        int fillSoundId,
        int variationSoundId,
        int outroSoundId
    ){
        if(mainSoundId < 1){
            std::cerr << "DynamicSong::addSection() mainSoundId has to be valid (1 or bigger)" << std::endl;
            return;
        }
        sections.push_back(Section(mainSoundId, fillSoundId, variationSoundId, outroSoundId));
    }

    std::vector<int> getCurrentSectionCurrentBeatAudios(){
        if(currentSection >= static_cast<int>(sections.size())){
            std::cerr << "getCurrentSectionCurrentBeatAudios() section doesnt exist" << std::endl;
            return {};
        }
        return { decideAudio() };
    }

    // int getCurrentBeat(){
    //     if(currentSection >= static_cast<int>(sections.size())){
    //         return 0;
    //     }
    //     return sections[currentSection].beat;
    // }

    void triggerSectionChange(int sectionToTrigger){
        std::cout << "triggerSectionChange() sectionToTrigger " << sectionToTrigger << std::endl;
        nextSection = sectionToTrigger;
    }

private:
    int currentSection = 0;
    int nextSection = 0;
    std::vector<Section> sections = {};

    int decideAudio(){
        int currentBeat = sections[currentSection].beat;
        std::cout << "section " << currentSection << ". nextSection " << nextSection << ". beat " << currentBeat << std::endl;
        sections[currentSection].beat += 1;
        if(sections[currentSection].beat >= 4){
            sections[currentSection].beat = 0;
        }
        bool isSectionEnding = currentSection != nextSection;
        
        int currentMainSoundId = sections[currentSection].mainSoundId;
        int currentFillSoundId = sections[currentSection].fillSoundId;
        int currentVariationSoundId = sections[currentSection].variationSoundId;
        int currentOutroSoundId = sections[currentSection].outroSoundId;

        if(currentBeat == 1){
            if(isSectionEnding){
                currentSection = nextSection;
                if(currentOutroSoundId > 0){
                    return currentOutroSoundId;
                }
            }
            if(currentVariationSoundId > 0){
                return currentVariationSoundId;
            }
        }

        if(currentBeat == 3){
            if(isSectionEnding){
                currentSection = nextSection;
                if(currentOutroSoundId > 0){
                    return currentOutroSoundId;
                }
            }
            if(currentFillSoundId > 0){
                return currentFillSoundId;
            }
        }

        return currentMainSoundId;
    }

};
