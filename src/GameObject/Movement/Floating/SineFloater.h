#pragma once

#include "GameObject/GameObject.h"
#include "FloatingComponent.h"
#include <memory>

template <size_t N>
class SineFloater: public FloatingComponent {
private:
    const std::array<float, N>& sineTable;
    float scale;
    float currentValue;
    float lastValue;
    int currentIndex;

public:
    SineFloater(const std::array<float, N>& table, float scale): sineTable(table), scale(scale), currentValue(0.0f), lastValue(table[0]), currentIndex(-1) {
        //
    }

    float getDelta(){
        return (currentValue - lastValue) * scale;
    }

    void update() override {
        lastValue = currentValue;
        currentIndex = (currentIndex + 1) % N;
        currentValue = sineTable[currentIndex];
    }

    std::unique_ptr<FloatingComponent> clone() const override {
        return std::make_unique<SineFloater>(sineTable, scale);
    }
};
