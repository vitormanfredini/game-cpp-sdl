#pragma once

#include <functional>
#include <memory>

class LevelManager {

private:
    int level;
    float gemValue;
    float maxGemValue;

    std::function<void(int)> onAdvanceLevelCallback;

    void recalculateCurrentLevelMax(){
        maxGemValue = static_cast<float>(level + 1);
    }

public:

    LevelManager(): gemValue(0.0f), level(0), maxGemValue(0.0f) {
        recalculateCurrentLevelMax();
    }

    LevelManager(int level, float gemValue, float maxGemValue): level(level), gemValue(gemValue), maxGemValue(maxGemValue) {
        recalculateCurrentLevelMax();
    }

    void addGemValue(float value){
        gemValue += value;
        if(gemValue < maxGemValue){
            return;
        }

        level += 1;
        gemValue -= maxGemValue;
        recalculateCurrentLevelMax();

        if(onAdvanceLevelCallback){
            onAdvanceLevelCallback(level);
        }
    }

    float getLevelPercentage(){
        return gemValue / maxGemValue;
    }

    void setAdvanceLevelCallback(std::function<void(int)> onAdvanceLevel) {
        onAdvanceLevelCallback = onAdvanceLevel;
    }

    std::unique_ptr<LevelManager> clone() {
        return std::make_unique<LevelManager>(level, gemValue, maxGemValue);
    }

};
