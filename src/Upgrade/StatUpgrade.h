#pragma once
#include "GameObject/Character/CharacterStat.h"
#include <memory>

class StatUpgrade {
private:
    CharacterStat type;
    float value;
public:
    StatUpgrade(CharacterStat type, float value): type(type), value(value) {
        //
    }
    CharacterStat getType(){
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