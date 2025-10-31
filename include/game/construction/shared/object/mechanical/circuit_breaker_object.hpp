#pragma once
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/mechanical/circuit_breaker_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/entity.hpp"


namespace Assembled
{
    struct CircuitBreakerData
    {
        entt::entity entity = entt::null;
        entt::entity indicator = entt::null;
        Component::CircuitBreaker& breaker;
    };
}


namespace Construct
{
    Assembled::CircuitBreakerData CircuitBreakerObject(
        entt::registry& registry,
        Component::Transform&& transform,
        const entt::entity system
    ) noexcept;
}