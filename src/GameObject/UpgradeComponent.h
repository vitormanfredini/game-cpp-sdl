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
    std::string description;
public:
    UpgradeComponent(UpgradeId id, StatType type, int level, float value, std::string description): id(id), type(type), level(level), value(value), description(description) {
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
    virtual std::string& getDescription(){
        return description;
    }
    virtual ~UpgradeComponent() = default;
    virtual std::unique_ptr<UpgradeComponent> clone() {
        return std::make_unique<UpgradeComponent>(id, type, level, value, description);
    };
};