#ifndef __WINDY_RESOURCES_H__
#define __WINDY_RESOURCES_H__

#include <string>

namespace game {

enum class ResourceKind {
    EnemyGeneral,
    EnemyMilitary,
    EnemySheriff,
    EnemyNight,
    EnergyBar,
    Item,
    LevelFX,
    LevelSpecial,
    LevelUI,
    PauseMenu,
    Player,
    Weapon,
};

struct Resources {
    const std::string _spritePath;
    const std::string _armaturePath;
    const ResourceKind _kind;
    const std::string _entityName;
    const bool _hasArmature;

    Resources(ResourceKind kind, std::string entityName, bool hasArmature = true) noexcept;

    void cache() const noexcept;
    static std::string entityPath(ResourceKind kind) noexcept;

private:
    mutable bool _cached = false;
};

}

#endif
