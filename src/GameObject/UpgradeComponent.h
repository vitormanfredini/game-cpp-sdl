#pragma once
#include "UpgradeType.h"
#include <memory>

class UpgradeComponent {
private:
    float value;
    UpgradeType type;
public:
    UpgradeComponent(UpgradeType type, float value): type(type), value(value) {
        //
    }
    virtual UpgradeType getType(){
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