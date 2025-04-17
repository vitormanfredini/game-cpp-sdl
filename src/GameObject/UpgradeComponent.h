#pragma once
#include "StatType.h"
#include "UpgradeId.h"
#include <memory>

class UpgradeComponent {
private:
    UpgradeId id;
    float value;
    StatType type;
    int level;
public:
    UpgradeComponent(UpgradeId id, StatType type, int level, float value): id(id), type(type), level(level), value(value) {
        //
    }
    virtual UpgradeId getId(){
        return id;
    };
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
        return std::make_unique<UpgradeComponent>(id, type, level, value);
    };
};