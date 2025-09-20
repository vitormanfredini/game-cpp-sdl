#pragma once

#include "UpgradeComponent.h"
#include "UpgradeId.h"

struct UpgradeOption {
    UpgradeId id;
    UpgradeComponent::Type type;
    std::string description;

    UpgradeOption(UpgradeId id, UpgradeComponent::Type type, std::string description) : id(id), type(type), description(description) {}
};
