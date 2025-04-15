#pragma once
#include "StatType.h"
#include <memory>

class UpgradeComponent {
private:
    float value;
    StatType type;
    int level;
public:
    UpgradeComponent(StatType type, int level, float value): type(type), level(level), value(value) {
        //
    }
    virtual StatType getType(){
        return type;
    };
    virtual float getValue(){
        return value;
    };
    virtual int getLevel(){
        return level;
    }
    virtual ~UpgradeComponent() = default;
    virtual std::unique_ptr<UpgradeComponent> clone() {
        return std::make_unique<UpgradeComponent>(type, level, value);
    };
};