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

class UpgradeFactory {

public:

    UpgradeFactory(){

        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 1, 0.2, "Aumenta a vida máxima em 20%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 2, 0.1, "Aumenta a vida máxima em 10%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, StatType::MaxHealth, 3, 0.08, "Aumenta a vida máxima em 8%"));

        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 1, 0.2, "Aumenta a velocidade base em 20%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 2, 0.1, "Aumenta a velocidade base em 10%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, StatType::BaseSpeed, 3, 0.08, "Aumenta a velocidade base em 8%"));

        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 1, 0.03, "Aumenta a regeneração em 3"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 2, 0.02, "Aumenta a regeneração em 2"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 3, 0.01, "Aumenta a regeneração em 1"));

        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 1, 30.0, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 2, 30.0, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 3, 20.0, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 4, 20.0, "Diminui o tempo para regenerar"));

        availableUpgradeIds = {};
        for (const auto & [ upgradeId, value ] : prototypes) {
            availableUpgradeIds.push_back(upgradeId);
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

};
