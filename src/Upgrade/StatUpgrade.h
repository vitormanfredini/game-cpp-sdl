#pragma once
#include "GameObject/Character/StatType.h"
#include <memory>

class StatUpgrade {
private:
    float value;
    StatType type;
public:
    StatUpgrade(StatType type, float value): type(type), value(value) {
        //
    }
    StatType getType(){
        return type;
    };
    float getValue(){
        return value;
    };
    ~StatUpgrade() = default;
    std::unique_ptr<StatUpgrade> clone() {
        return std::make_unique<StatUpgrade>(type, value);
    };
};