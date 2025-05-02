#pragma once
#include "Weapons/WeaponStat.h"
#include <memory>

class WeaponUpgrade {
private:
    float value;
    WeaponStat type;
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