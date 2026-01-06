#pragma once
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"


namespace Entity
{

class LightPoint final
{
public:
    static entt::entity Create(
        entt::registry& registry,
        Scene boundScene,
        Nc::Vector2f position,
        Nc::RGBa color,
        float strength,
        float range
    ) noexcept;
};

}