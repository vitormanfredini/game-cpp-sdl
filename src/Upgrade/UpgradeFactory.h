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
#include "AsciiArt.h"

class UpgradeFactory {

public:

    UpgradeFactory(ItemFactory* itemFactory, WeaponFactory* weaponFactory): itemFactory(itemFactory) {

        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.2), 1, asciiArt.get(AsciiArt::Id::MaxHealth)));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.1), 2, asciiArt.get(AsciiArt::Id::MaxHealth)));
        prototypes[UpgradeId::MaxHealth].push_back(std::make_unique<UpgradeComponent>(UpgradeId::MaxHealth, std::make_unique<StatUpgrade>(CharacterStat::MaxHealth, 0.08), 3, asciiArt.get(AsciiArt::Id::MaxHealth)));

        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.1), 1, asciiArt.get(AsciiArt::Id::RunFaster)));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.06), 2, asciiArt.get(AsciiArt::Id::RunFaster)));
        prototypes[UpgradeId::BaseSpeed].push_back(std::make_unique<UpgradeComponent>(UpgradeId::BaseSpeed, std::make_unique<StatUpgrade>(CharacterStat::BaseSpeed, 0.03), 3, asciiArt.get(AsciiArt::Id::RunFaster)));

        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.03), 1, asciiArt.get(AsciiArt::Id::HealthRegenerateMore)));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.02), 2, asciiArt.get(AsciiArt::Id::HealthRegenerateMore)));
        prototypes[UpgradeId::RegenerateHealthAmount].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthAmount, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthAmount, 0.01), 3, asciiArt.get(AsciiArt::Id::HealthRegenerateMore)));

        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 30.0), 1, asciiArt.get(AsciiArt::Id::HealthRegenerateFaster)));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 30.0), 2, asciiArt.get(AsciiArt::Id::HealthRegenerateFaster)));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 20.0), 3, asciiArt.get(AsciiArt::Id::HealthRegenerateFaster)));
        prototypes[UpgradeId::RegenerateHealthFasterInUpdates].push_back(std::make_unique<UpgradeComponent>(UpgradeId::RegenerateHealthFasterInUpdates, std::make_unique<StatUpgrade>(CharacterStat::RegenerateHealthFasterInUpdates, 20.0), 4, asciiArt.get(AsciiArt::Id::HealthRegenerateFaster)));

        prototypes[UpgradeId::Weapon].push_back(std::make_unique<UpgradeComponent>(UpgradeId::Weapon, weaponFactory->create(WeaponId::FireBall), 1, asciiArt.get(AsciiArt::Id::FireBalls)));

        prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -13.0f, WeaponId::Sword), 2, asciiArt.get(AsciiArt::Id::LowCutUpgradeFireRate)));
        prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -13.0f, WeaponId::Sword), 3, asciiArt.get(AsciiArt::Id::LowCutUpgradeFireRate2)));
    }

    std::vector<UpgradeOption> createRandomUpgradeOptions(size_t max, std::vector<WeaponId> weaponIds){

        auto& itemsVector = prototypes[UpgradeId::Item];
        while (itemsVector.size() < max) {
            itemsVector.push_back(std::make_unique<UpgradeComponent>(UpgradeId::Item, itemFactory->create(ItemId::Health), 1, "Poção de vida"));
            itemsVector.push_back(std::make_unique<UpgradeComponent>(UpgradeId::Item, itemFactory->create(ItemId::Gem), 1, "Gem"));
        }

        std::shuffle(std::begin(prototypes[UpgradeId::Weapon]), std::end(prototypes[UpgradeId::Weapon]), randomEngine);

        std::vector<UpgradeId> allAvailableUpgradeIds = {};
        for (int c = 0; c < (int)UpgradeId::COUNT; c++) {
            UpgradeId id = static_cast<UpgradeId>(c);
            if(id == UpgradeId::Item){
                continue;
            }
            if(prototypes.find(id) == prototypes.end()){
                continue;
            }
            if(prototypes[id].size() == 0){
                continue;
            }
            allAvailableUpgradeIds.push_back(id);
        }

        std::shuffle(std::begin(allAvailableUpgradeIds), std::end(allAvailableUpgradeIds), randomEngine);

        std::vector<UpgradeOption> options = {};

        size_t maxUpgrades = std::min(allAvailableUpgradeIds.size(), static_cast<size_t>(max));
        for(size_t c=0; c<maxUpgrades; c++){

            size_t internalIndex = 0;
            if(allAvailableUpgradeIds[c] == UpgradeId::WeaponUpgrade){
                std::vector<size_t> possibleInternalIndex = {};
                WeaponId lastWeaponId;
                for(size_t d=0; d<prototypes[UpgradeId::WeaponUpgrade].size(); d++){
                    WeaponId weaponId = prototypes[UpgradeId::WeaponUpgrade][d]->getWeaponUpgrade()->getWeaponId();
                    if(possibleInternalIndex.empty() || lastWeaponId != weaponId){
                        lastWeaponId = weaponId;
                        possibleInternalIndex.push_back(d);
                    }
                }

                if(possibleInternalIndex.size() > 0){
                    std::shuffle(std::begin(possibleInternalIndex), std::end(possibleInternalIndex), randomEngine);
                    internalIndex = possibleInternalIndex[0];
                }
            }

            options.push_back({
                allAvailableUpgradeIds[c],
                internalIndex,
                prototypes[allAvailableUpgradeIds[c]][internalIndex]->getType(),
                prototypes[allAvailableUpgradeIds[c]][internalIndex]->getDescription(options.size()+1)
            });
        }

        for(size_t c=0;options.size() < max; c++){
            options.push_back({
                UpgradeId::Item,
                c,
                prototypes[UpgradeId::Item].front()->getType(),
                prototypes[UpgradeId::Item].front()->getDescription(options.size()+1)
            });
        }

        return options;
    }

    std::unique_ptr<UpgradeComponent> redeemUpgrade(UpgradeOption option){

        UpgradeId upgradeId = option.id;
        if(prototypes[upgradeId].empty()){
            std::cerr << "Trying to redeem upgrade from an empty vector" << std::endl;
        }

        size_t internalIndex = option.internalIndex;
        if(internalIndex >= prototypes[upgradeId].size()){
            std::cerr << "Trying to redeem upgrade with invalid index" << std::endl;
        }

        std::unique_ptr<UpgradeComponent> redeemedUpgrade = std::move(prototypes[upgradeId][internalIndex]);
        prototypes[upgradeId].erase(prototypes[upgradeId].begin() + internalIndex);

        return redeemedUpgrade;
    }

    void enableUpgradesForWeapon(WeaponId weaponId){
        if(weaponId == WeaponId::FireBall){
            prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -4.0f, WeaponId::FireBall), 2, "Fireball mais rápida"));
            prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::Attack, 1.0f, WeaponId::FireBall), 3, "Fireball mais forte"));
            prototypes[UpgradeId::WeaponUpgrade].push_back(std::make_unique<UpgradeComponent>(UpgradeId::WeaponUpgrade, std::make_unique<WeaponUpgrade>(WeaponStat::FiringRate, -5.0f, WeaponId::FireBall), 4, "Fireball MUITO rápida"));
        }
    }

private:
    std::unordered_map<UpgradeId,std::vector<std::unique_ptr<UpgradeComponent>>> prototypes;
    ItemFactory* itemFactory;
    std::default_random_engine randomEngine = {};
    AsciiArt asciiArt;
};
