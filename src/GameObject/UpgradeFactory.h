#pragma once

#include <memory>
#include "UpgradeComponent.h"
#include "UpgradeId.h"
#include "StatType.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <random>

class UpgradeFactory {

public:

    UpgradeFactory(){

        upgradesLastIndexesUsed[UpgradeId::MaxHealth] = 0;
        upgrades[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 1, 0.2));
        upgrades[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 2, 0.1));
        upgrades[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(StatType::MaxHealth, 3, 0.08));

        upgradesLastIndexesUsed[UpgradeId::BaseSpeed] = 0;
        upgrades[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(StatType::BaseSpeed, 1, 0.2));
        upgrades[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(StatType::BaseSpeed, 2, 0.1));
        upgrades[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(StatType::BaseSpeed, 3, 0.08));

        availableUpgrades = {
            UpgradeId::MaxHealth,
            UpgradeId::BaseSpeed
        };
    }

    std::vector<UpgradeId> findRandomUpgradesStillAvailable(int howMany){
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(availableUpgrades), std::end(availableUpgrades), rng);

        std::vector<UpgradeId> limitedAvailableUpgrades = availableUpgrades;
        if (limitedAvailableUpgrades.size() > 3) {
            limitedAvailableUpgrades.erase(limitedAvailableUpgrades.begin() + howMany, limitedAvailableUpgrades.end());
        }
        
        return limitedAvailableUpgrades;
    }

    std::unique_ptr<UpgradeComponent> createNext(UpgradeId upgradeId) {
        upgradesLastIndexesUsed[upgradeId] += 1;
        std::unique_ptr<UpgradeComponent> newUpgrade = upgrades[upgradeId][upgradesLastIndexesUsed[upgradeId]-1]->clone();

        if(upgradesLastIndexesUsed[upgradeId] >= upgrades[upgradeId].size()){
            availableUpgrades.erase(std::remove(availableUpgrades.begin(), availableUpgrades.end(), upgradeId), availableUpgrades.end());
        }

        return newUpgrade;
    }

    

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> upgrades;
    std::unordered_map<UpgradeId,int> upgradesLastIndexesUsed;
    std::vector<UpgradeId> availableUpgrades;

};
