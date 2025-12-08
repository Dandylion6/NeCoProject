#pragma once
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"


namespace Construct
{
    const entt::entity LightPointEntity(
        entt::registry& registry, 
        Nc::Vector2f position, 
        Scene boundScene, 
        Nc::RGBa color, 
        float strength, 
        float range
    );
}