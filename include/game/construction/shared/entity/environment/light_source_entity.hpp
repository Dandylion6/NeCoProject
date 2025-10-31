#pragma once
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"


namespace Construct
{
    const entt::entity LightSourceEntity(
        entt::registry& registry, 
        Nc::Vector2f position, 
        Scene boundScene, 
        Nc::RGBa color, 
        float strength, 
        float range
    );
}