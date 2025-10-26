#include "assemblers/entities/interactive/circuit_breaker_object.hpp"
#include "assemblers/entities/interactive/lever_entity.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/bounds.hpp"


namespace Construct
{
    static const entt::entity BreakerIndicatorEntity(
        entt::registry& registry,
        Scene scene,
        Nc::Vector2f breakerCenterPosition
    ) noexcept
    {
        constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(20.0f);

        const entt::entity entity = registry.create();

        registry.emplace<Component::Transform>(entity, scene, breakerCenterPosition + Nc::Vector2f::Left(80.0f), SIZE, SIZE * 0.5f);
        registry.emplace<Component::Rectangle>(entity, WHITE);

        return entity;
    }


    Assembled::CircuitBreakerData CircuitBreakerObject(
        entt::registry& registry, 
        Component::Transform&& transform,
        const entt::entity system
    ) noexcept
    {
        Scene scene = transform.boundScene;
        Assembled::LeverData leverData = Construct::LeverEntity(registry, std::move(transform), 30.0f, "", "");

        Nc::Vector2f centerPosition = Nc::Bounds::CenterOf(Nc::Bounds(transform));
        const entt::entity indicator = BreakerIndicatorEntity(registry, scene, centerPosition);

        Component::CircuitBreaker& breaker = registry.emplace<Component::CircuitBreaker>(leverData.entity, system, indicator);

        return { leverData.entity, indicator, breaker };
    }
}