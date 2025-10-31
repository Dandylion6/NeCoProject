#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct GameState;


namespace Construct 
{
    const entt::entity SettingsBackgroundEntity(
        entt::registry& registry, GameState& gameState, Nc::Vector2f windowSize
    );
}