#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


namespace Construct 
{
    const entt::entity SettingsBackgroundEntity(
        entt::registry& registry, Nc::Vector2f windowSize
    );
}