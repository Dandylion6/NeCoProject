#pragma once
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/color.hpp"
#include "utility/vector2.hpp"


namespace Construct
{
    const entt::entity LightSourceEntity(
        entt::registry& registry, Nc::Vector2f position, Scene boundScene, Nc::RGBa color, float strength, float range
    );
}