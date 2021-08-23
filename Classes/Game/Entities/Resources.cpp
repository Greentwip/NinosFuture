#include "Resources.h"

#include "Windy/Armature.h"
#include "Windy/Sprite.h"

#include <type_traits>

namespace game {

Resources::Resources(ResourceKind kind, std::string entityName, bool hasArmature) noexcept:
        _spritePath{"sprites/" + Resources::entityPath(kind) + "/" + entityName + "/" + entityName},
        _armaturePath{"physics/" + Resources::entityPath(kind) + "/" + entityName + "/" + entityName},
        _kind{kind},
        _entityName{std::move(entityName)},
        _hasArmature{hasArmature} {
    cache();
}

void Resources::cache() const noexcept {
    if (!_cached) {
        windy::Sprite::cache(_spritePath);
        if (_hasArmature) {
            windy::Armature::cache(_armaturePath);
        }
        _cached = true;
    }
}

std::string Resources::entityPath(ResourceKind kind) noexcept {
    switch (kind) {
        case ResourceKind::EnemyGeneral:
            return "characters/enemy/general";
        case ResourceKind::EnemyMilitary:
            return "characters/enemy/military";
        case ResourceKind::EnemySheriff:
            return "characters/enemy/sheriff";
        case ResourceKind::EnemyNight:
            return "characters/enemy/night";
        case ResourceKind::EnergyBar:
            return "gameplay/level/ui/energy_bar";
        case ResourceKind::Item:
            return "gameplay/level/goods";
        case ResourceKind::LevelFX:
            return "gameplay/level/fx";
        case ResourceKind::LevelSpecial:
            return "gameplay/level/special/general";
        case ResourceKind::LevelUI:
            return "gameplay/level/ui";
        case ResourceKind::PauseMenu:
            return "gameplay/screens/pause_menu";
        case ResourceKind::Player:
            return "characters/player/regular";
        case ResourceKind::Weapon:
            return "gameplay/level/weapon";
    }
}

}
