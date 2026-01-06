#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/mechanical/circuit_breaker_component.hpp"
#include "game/state/scene.hpp"


namespace Object
{

class CircuitBreaker final
{
public:
    struct Data final
    {
        Component::Logic::CircuitBreaker& breaker;
        entt::entity entity = entt::null;
        entt::entity indicator = entt::null;
    };


    static Data Create(
        entt::registry& registry,
        Component::Transform&& transform,
        entt::entity system
    ) noexcept;

private:
    class Indicator final
    {
    public:
        static entt::entity Create(
            entt::registry& registry,
            Scene scene,
            Nc::Vector2f breakerCenterPosition
		) noexcept;

    };

};

}