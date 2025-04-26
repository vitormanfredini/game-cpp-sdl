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

#include "StatUpgrade.h"

class UpgradeFactory {

public:

    UpgradeFactory(){

        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(StatType::MaxHealth, 0.2), 1, "Aumenta a vida máxima em 20%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(StatType::MaxHealth, 0.1), 2, "Aumenta a vida máxima em 20%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(StatType::MaxHealth, 0.08), 3, "Aumenta a vida máxima em 20%"));

        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(StatType::BaseSpeed, 0.1), 1, "Aumenta a velocidade em 10%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(StatType::BaseSpeed, 0.06), 2, "Aumenta a velocidade em 6%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(StatType::BaseSpeed, 0.03), 3, "Aumenta a velocidade em 3%"));

        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(StatType::RegenerateHealthAmount, 0.03), 1, "Aumenta a regeneração em 3"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(StatType::RegenerateHealthAmount, 0.02), 2, "Aumenta a regeneração em 2"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(StatType::RegenerateHealthAmount, 0.01), 3, "Aumenta a regeneração em 1"));

        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(StatType::RegenerateHealthFasterInUpdates, 30.0), 1, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(StatType::RegenerateHealthFasterInUpdates, 30.0), 2, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(StatType::RegenerateHealthFasterInUpdates, 20.0), 3, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(StatType::RegenerateHealthFasterInUpdates, 20.0), 4, "Diminui o tempo para regenerar"));

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
