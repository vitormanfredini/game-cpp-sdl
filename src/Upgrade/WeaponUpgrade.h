#pragma once
#include "GameObject/Character/Weapons/WeaponStat.h"
#include <memory>

class WeaponUpgrade {
private:
    WeaponStat type;
    float value;
public:
    WeaponUpgrade(WeaponStat type, float value): type(type), value(value) {
        //
    }
    WeaponStat getType(){
        return type;
    };
    float getValue(){
        return value;
    };
    ~WeaponUpgrade() = default;
    std::unique_ptr<WeaponUpgrade> clone() {
        return std::make_unique<WeaponUpgrade>(type, value);
    };
};