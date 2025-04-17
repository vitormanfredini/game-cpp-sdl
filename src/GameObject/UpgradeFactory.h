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

        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 1, 0.2));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 2, 0.1));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 3, 0.08));

        
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 1, 0.2));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 2, 0.1));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 3, 0.08));

        availableUpgradeIds = {
            UpgradeId::MaxHealth,
            UpgradeId::BaseSpeed
        };

        for(UpgradeId upgradeId : availableUpgradeIds){
            upgradesTimesConsumed[upgradeId] = 0;
        }

    }

    std::vector<std::unique_ptr<UpgradeComponent>> createRandomUpgrades(int max){
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(availableUpgradeIds), std::end(availableUpgradeIds), rng);

        std::vector<UpgradeId> limitedAvailableUpgradeIds = availableUpgradeIds;
        if (limitedAvailableUpgradeIds.size() > max) {
            limitedAvailableUpgradeIds.erase(limitedAvailableUpgradeIds.begin() + max, limitedAvailableUpgradeIds.end());
        }

        std::vector<std::unique_ptr<UpgradeComponent>> randomUpgrades = {};
        for(UpgradeId upgradeId : limitedAvailableUpgradeIds){
            randomUpgrades.push_back(std::move(prototypes[upgradeId][upgradesTimesConsumed[upgradeId]]->clone()));
        }

        return randomUpgrades;
    }

    void playerChoseThisUpgrade(UpgradeComponent* upgrade){
        UpgradeId upgradeId = upgrade->getId();
        upgradesTimesConsumed[upgradeId] += 1;

        if(upgradesTimesConsumed[upgradeId] >= prototypes[upgradeId].size()){
            availableUpgradeIds.erase(std::remove(availableUpgradeIds.begin(), availableUpgradeIds.end(), upgradeId), availableUpgradeIds.end());
        }
    }

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> prototypes;
    std::unordered_map<UpgradeId,int> upgradesTimesConsumed;
    std::vector<UpgradeId> availableUpgradeIds;

    std::unique_ptr<UpgradeComponent> create(UpgradeId upgradeId) {
        upgradesTimesConsumed[upgradeId] += 1;
        std::unique_ptr<UpgradeComponent> newUpgrade = prototypes[upgradeId][upgradesTimesConsumed[upgradeId]-1]->clone();
        return newUpgrade;
    }

};
