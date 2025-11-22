#pragma once
#include "GameObject/Character/Weapons/WeaponStat.h"
#include "GameObject/Character/Weapons/WeaponId.h"
#include <memory>

class WeaponUpgrade {
private:
    WeaponStat type;
    float value;
    WeaponId weaponId;
public:
    WeaponUpgrade(WeaponStat type, float value, WeaponId weaponId): type(type), value(value), weaponId(weaponId) {
        //
    }
    WeaponStat getType(){
        return type;
    };
    float getValue(){
        return value;
    };
    WeaponId getWeaponId(){
        return weaponId;
    };
    ~WeaponUpgrade() = default;
    std::unique_ptr<WeaponUpgrade> clone() {
        return std::make_unique<WeaponUpgrade>(type, value, weaponId);
    };
};