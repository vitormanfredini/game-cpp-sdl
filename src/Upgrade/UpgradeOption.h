#pragma once

#include "UpgradeComponent.h"
#include "UpgradeId.h"

struct UpgradeOption {
    UpgradeId id;
    size_t internalIndex;
    UpgradeComponent::Type type;
    std::string description;

    UpgradeOption(
        UpgradeId id,
        size_t internalIndex,
        UpgradeComponent::Type type,
        std::string description
    ) : id(id),
        internalIndex(internalIndex),
        type(type),
        description(description) {}
};
