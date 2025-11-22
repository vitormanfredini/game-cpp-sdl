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
#include "GameObject/Character/Weapons/WeaponId.h"
#include "GameObject/Character/Weapons/WeaponStat.h"
#include "GameObject/Character/Weapons/WeaponFactory.h"
#include "Audio/AudioEngine.h"
#include "StatUpgrade.h"

class UpgradeFactory {

public:

    UpgradeFactory(ItemFactory* itemFactory, WeaponFactory* weaponFactory): itemFactory(itemFactory) {

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

        prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -4.0f), 1, "Atirar mais rápido"));
        prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -3.0f), 2, "Atirar mais rápido"));

        prototypes[UpgradeId::Weapon].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Weapon, weaponFactory->create(WeaponId::FireBall), 1, "Lança bolas de fogo"));
        prototypes[UpgradeId::Weapon].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Weapon, weaponFactory->create(WeaponId::FireBall), 2, "Lança bolas de fogo 2 teste"));

    }

    std::vector<UpgradeOption> createRandomUpgradeOptions(int max){

        std::shuffle(std::begin(prototypes[UpgradeId::Weapon]), std::end(prototypes[UpgradeId::Weapon]), randomEngine);

        std::vector<int> upgradeIdIndexes(prototypes.size());
        std::iota(upgradeIdIndexes.begin(), upgradeIdIndexes.end(), 0);
        std::shuffle(upgradeIdIndexes.begin(), upgradeIdIndexes.end(), randomEngine);

        std::vector<UpgradeOption> options = {};

        size_t maxUpgrades = std::min(upgradeIdIndexes.size(), static_cast<size_t>(max));
        for(size_t c=0; c < maxUpgrades; c++){
            UpgradeId upgradeId = static_cast<UpgradeId>(upgradeIdIndexes[c]);
            options.push_back({
                upgradeId,
                prototypes[upgradeId].front()->getType(),
                prototypes[upgradeId].front()->getDescription()
            });
        }

        return options;
    }

    std::unique_ptr<UpgradeComponent> redeemUpgrade(UpgradeOption option){

        UpgradeId upgradeId = option.id;
        if(prototypes[upgradeId].empty()){
            std::cerr << "Trying to redeem upgrade from an empty vector" << std::endl;
        }

        std::unique_ptr<UpgradeComponent> redeemedUpgrade = std::move(prototypes[upgradeId].front());
        prototypes[upgradeId].erase(prototypes[upgradeId].begin());

        return redeemedUpgrade;
    }

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> prototypes;
    ItemFactory* itemFactory;
    std::default_random_engine randomEngine = {};
};
