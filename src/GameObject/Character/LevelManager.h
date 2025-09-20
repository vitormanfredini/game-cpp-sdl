#pragma once

#include <functional>
#include <memory>

class LevelManager {

private:
    float gemValue;
    float maxGemValue;
    int level;

    std::function<void(int)> onAdvanceLevelCallback;

    void recalculateCurrentLevelMax(){
        maxGemValue = static_cast<float>(level + 1);
    }

public:

    LevelManager(): gemValue(0.0f), maxGemValue(0.0f), level(0) {
        recalculateCurrentLevelMax();
    }

    LevelManager(float gemValue, float maxGemValue, int level): gemValue(gemValue), maxGemValue(maxGemValue), level(level) {
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

    void setOnAdvanceLevelCallback(std::function<void(int)> onAdvanceLevel) {
        onAdvanceLevelCallback = onAdvanceLevel;
    }

    std::unique_ptr<LevelManager> clone() {
        return std::make_unique<LevelManager>(gemValue, maxGemValue, level);
    }

};
