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

        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 1, 0.01));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 2, 0.01));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, StatType::RegenerateHealthAmount, 3, 0.01));

        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 1, 10.0));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 2, 10.0));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, StatType::RegenerateHealthFasterInUpdates, 3, 5.0));

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

        if(upgrade->getId() == UpgradeId::MaxHealth){
            std::cout << "New upgrade: MaxHealth. Value: " << upgrade->getValue() << ". Level: " << upgrade->getLevel() << "." << std::endl;
        }
        if(upgrade->getId() == UpgradeId::BaseSpeed){
            std::cout << "New upgrade: BaseSpeed. Value: " << upgrade->getValue() << ". Level: " << upgrade->getLevel() << "." << std::endl;
        }
        if(upgrade->getId() == UpgradeId::RegenerateHealthAmount){
            std::cout << "New upgrade: RegenerateHealthAmount. Value: " << upgrade->getValue() << ". Level: " << upgrade->getLevel() << "." << std::endl;
        }
        if(upgrade->getId() == UpgradeId::RegenerateHealthFasterInUpdates){
            std::cout << "New upgrade: RegenerateHealthFasterInUpdates. Value: " << upgrade->getValue() << ". Level: " << upgrade->getLevel() << "." << std::endl;
        }

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
