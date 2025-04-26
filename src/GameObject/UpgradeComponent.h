#pragma once
#include "StatType.h"
#include "StatUpgrade.h"
#include "UpgradeId.h"
#include <memory>
#include <variant>
#include <iostream>

class UpgradeComponent {
public:
    enum class Type {
        StatUpgrade
        // WeaponUpgrade,
        // Item
    };
private:
    Type type;
    std::unique_ptr<StatUpgrade> statUpgrade;
    // std::unique_ptr<WeaponUpgrade> weaponUpgrade;
    // std::unique_ptr<Item> item;

    UpgradeId id;
    int level;
    std::string description;

public:
    explicit UpgradeComponent(UpgradeId id, std::unique_ptr<StatUpgrade> statUpgrade, int level, std::string description)
        : type(Type::StatUpgrade), statUpgrade(std::move(statUpgrade)), id(id), level(level), description(description) {
            //
        }

    UpgradeComponent(UpgradeComponent&& other) noexcept
        : type(other.type),
          statUpgrade(std::move(other.statUpgrade)),
          level(other.level),
          description(other.description)

        //   weaponUpgrade(std::move(other.weaponUpgrade)),
        //   item(std::move(other.item)) 
          {

    }

    Type getType() const { return type; }

    StatUpgrade* getStatUpgrade() const {
        return statUpgrade.get();
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
        if(type == Type::StatUpgrade){
            return std::make_unique<UpgradeComponent>(id, statUpgrade->clone(), level, description);
        }
        std::cerr << "std::unique_ptr<UpgradeComponent> clone(): unsupported Type" << std::endl;
    };
};