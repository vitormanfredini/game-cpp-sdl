#pragma once

#include <memory>
#include "UpgradeComponent.h"
#include "UpgradeId.h"
#include "UpgradeOption.h"
#include "GameObject/Character/CharacterStat.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include "Weapons/WeaponStat.h"

#include "StatUpgrade.h"

class UpgradeFactory {

public:

    UpgradeFactory(ItemFactory* itemFactory): itemFactory(itemFactory) {

        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.2), 1, "Aumenta a vida máxima em 20%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.1), 2, "Aumenta a vida máxima em 20%"));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.08), 3, "Aumenta a vida máxima em 20%"));

        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.1), 1, "Aumenta a velocidade em 10%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.06), 2, "Aumenta a velocidade em 6%"));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.03), 3, "Aumenta a velocidade em 3%"));

        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.03), 1, "Aumenta a regeneração em 3"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.02), 2, "Aumenta a regeneração em 2"));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.01), 3, "Aumenta a regeneração em 1"));

        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 30.0), 1, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 30.0), 2, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 20.0), 3, "Diminui o tempo para regenerar"));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 20.0), 4, "Diminui o tempo para regenerar"));

        prototypes[UpgradeId::Item].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Item, itemFactory->create(ItemId::Health), 1, "Poção de vida"));
        prototypes[UpgradeId::Item].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Item, itemFactory->create(ItemId::Gem), 1, "Gem"));
        prototypes[UpgradeId::Item].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Item, itemFactory->create(ItemId::Health), 1, "Poção de vida"));

        prototypes[UpgradeId::Weapon].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Weapon, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -2.0f), 1, "Atirar mais rápido"));
        prototypes[UpgradeId::Weapon].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Weapon, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -2.0f), 2, "Atirar mais rápido"));

        availableUpgradeIds = {};
        for (const auto & [ upgradeId, value ] : prototypes) {
            availableUpgradeIds.push_back(upgradeId);
            upgradesTimesConsumed[upgradeId] = 0;
        }

    }

    std::vector<std::unique_ptr<UpgradeOption>> createRandomUpgradeOptions(int max){
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(availableUpgradeIds), std::end(availableUpgradeIds), rng);

        std::vector<UpgradeId> limitedAvailableUpgradeIds = availableUpgradeIds;
        if (limitedAvailableUpgradeIds.size() > max) {
            limitedAvailableUpgradeIds.erase(limitedAvailableUpgradeIds.begin() + max, limitedAvailableUpgradeIds.end());
        }

        std::vector<std::unique_ptr<UpgradeOption>> options = {};
        for(UpgradeId upgradeId : limitedAvailableUpgradeIds){
            options.push_back(std::make_unique<UpgradeOption>(
                upgradeId,
                prototypes[upgradeId][upgradesTimesConsumed[upgradeId]]->getType(),
                prototypes[upgradeId][upgradesTimesConsumed[upgradeId]]->getDescription()
            ));
        }

        return options;
    }

    std::unique_ptr<UpgradeComponent> redeemUpgrade(UpgradeOption* option){

        UpgradeId upgradeId = option->id;
        upgradesTimesConsumed[upgradeId] += 1;

        if(upgradesTimesConsumed[upgradeId] >= prototypes[upgradeId].size()){
            availableUpgradeIds.erase(std::remove(availableUpgradeIds.begin(), availableUpgradeIds.end(), upgradeId), availableUpgradeIds.end());
        }

        return prototypes[upgradeId][upgradesTimesConsumed[upgradeId]-1]->clone();
    }

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> prototypes;
    std::unordered_map<UpgradeId,int> upgradesTimesConsumed;
    std::vector<UpgradeId> availableUpgradeIds;

    ItemFactory* itemFactory;
};
