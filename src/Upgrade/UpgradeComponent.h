#pragma once
#include "GameObject/Character/CharacterStat.h"
#include "GameObject/Item/Item.h"
#include "StatUpgrade.h"
#include "WeaponUpgrade.h"
#include "UpgradeId.h"
#include <memory>
#include <variant>
#include <iostream>

class UpgradeComponent {
public:
    enum class Type {
        Stat,
        Weapon,
        Item
    };
private:
    Type type;
    std::unique_ptr<StatUpgrade> statUpgrade;
    std::unique_ptr<WeaponUpgrade> weaponUpgrade;
    std::unique_ptr<Item> item;
    

    UpgradeId id;
    int level;
    std::string description;

public:

    explicit UpgradeComponent(UpgradeId id, std::unique_ptr<StatUpgrade> statUpgrade, int level, std::string description)
        : type(Type::Stat), statUpgrade(std::move(statUpgrade)), id(id), level(level), description(description) {}

    explicit UpgradeComponent(UpgradeId id, std::unique_ptr<WeaponUpgrade> weaponUpgrade, int level, std::string description)
        : type(Type::Weapon), weaponUpgrade(std::move(weaponUpgrade)), id(id), level(level), description(description) {}

    explicit UpgradeComponent(UpgradeId id, std::unique_ptr<Item> item, int level, std::string description)
        : type(Type::Item), item(std::move(item)), id(id), level(level), description(description) {}
    

    UpgradeComponent(UpgradeComponent&& other) noexcept
        : type(other.type),
          statUpgrade(std::move(other.statUpgrade)),
          level(other.level),
          description(other.description),
          item(std::move(other.item)),
          weaponUpgrade(std::move(other.weaponUpgrade))
          { }

    Type getType() const { return type; }

    StatUpgrade* getStatUpgrade() const {
        return statUpgrade.get();
    }

    WeaponUpgrade* getWeaponUpgrade() const {
        return weaponUpgrade.get();
    }

    Item* getItem() const {
        return item.get();
    }

    UpgradeId getId(){
        return id;
    };
    int getLevel(){
        return level;
    }
    std::string& getDescription(){
        return description;
    }
    ~UpgradeComponent() = default;
    std::unique_ptr<UpgradeComponent> clone() {
        if(type == Type::Stat){
            return std::make_unique<UpgradeComponent>(id, statUpgrade->clone(), level, description);
        }
        if(type == Type::Weapon){
            return std::make_unique<UpgradeComponent>(id, weaponUpgrade->clone(), level, description);
        }
        if(type == Type::Item){
            return std::make_unique<UpgradeComponent>(id, item->clone(), level, description);
        }
        std::cerr << "std::unique_ptr<UpgradeComponent> clone(): unsupported Type" << std::endl;
        return nullptr;
    };
};
