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
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 1, 0.2));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 2, 0.1));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 3, 0.08));

        upgradesLastIndexesUsed[UpgradeId::BaseSpeed] = 0;
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 1, 0.2));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 2, 0.1));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 3, 0.08));

        availableUpgrades = {
            UpgradeId::MaxHealth,
            UpgradeId::BaseSpeed
        };
    }

    std::vector<std::unique_ptr<UpgradeComponent>> createRandomUpgrades(int max){
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(availableUpgrades), std::end(availableUpgrades), rng);

        std::vector<UpgradeId> limitedAvailableUpgradeIds = availableUpgrades;
        if (limitedAvailableUpgradeIds.size() > max) {
            limitedAvailableUpgradeIds.erase(limitedAvailableUpgradeIds.begin() + max, limitedAvailableUpgradeIds.end());
        }

        std::vector<std::unique_ptr<UpgradeComponent>> randomUpgrades = {};
        for(UpgradeId upgradeId : limitedAvailableUpgradeIds){
            randomUpgrades.push_back(std::move(prototypes[upgradeId][upgradesLastIndexesUsed[upgradeId]]->clone()));
        }

        return randomUpgrades;
    }

    void playerChoseThisUpgrade(UpgradeComponent* upgrade){
        UpgradeId upgradeId = upgrade->getId();
        upgradesLastIndexesUsed[upgradeId] += 1;

        if(upgradesLastIndexesUsed[upgradeId] >= prototypes[upgradeId].size()){
            availableUpgrades.erase(std::remove(availableUpgrades.begin(), availableUpgrades.end(), upgradeId), availableUpgrades.end());
        }
    }

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> prototypes;
    std::unordered_map<UpgradeId,int> upgradesLastIndexesUsed;
    std::vector<UpgradeId> availableUpgrades;

    std::unique_ptr<UpgradeComponent> create(UpgradeId upgradeId) {
        upgradesLastIndexesUsed[upgradeId] += 1;
        std::unique_ptr<UpgradeComponent> newUpgrade = prototypes[upgradeId][upgradesLastIndexesUsed[upgradeId]-1]->clone();

        

        return newUpgrade;
    }

};
