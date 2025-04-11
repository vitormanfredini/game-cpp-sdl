#pragma once
#include "StatType.h"
#include <memory>

class UpgradeComponent {
private:
    float value;
    StatType type;
public:
    UpgradeComponent(StatType type, float value): type(type), value(value) {
        //
    }
    virtual StatType getType(){
        return type;
    };
    virtual float getValue(){
        return value;
    };
    virtual ~UpgradeComponent() = default;
    virtual std::unique_ptr<UpgradeComponent> clone() {
        return std::make_unique<UpgradeComponent>(type, value);
    };
};