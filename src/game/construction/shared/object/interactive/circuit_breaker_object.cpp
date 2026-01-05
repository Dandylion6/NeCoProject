#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/mechanical/circuit_breaker_component.hpp"
#include "game/construction/shared/entity/mechanical/lever_entity.hpp"
#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


Object::CircuitBreaker::Data Object::CircuitBreaker::Create(
    entt::registry& registry, 
    Component::Transform&& transform, 
    const entt::entity system
) noexcept
{
    Scene scene = transform.boundScene;
    Entity::Lever::Data leverData = Entity::Lever::Create(registry, std::move(transform), 30.0f, "", "");

    Nc::Vector2f centerPosition = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    const entt::entity indicator = Indicator::Create(registry, scene, centerPosition);

    Component::Logic::CircuitBreaker& breaker = registry.emplace<Component::Logic::CircuitBreaker>(
        leverData.entity, 
        system, 
        indicator
    );

    return { breaker, leverData.entity, indicator };
}


const entt::entity Object::CircuitBreaker::Indicator::Create(
    entt::registry& registry, 
    Scene scene, 
    Nc::Vector2f breakerCenterPosition
) noexcept
{
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(20.0f);

    const entt::entity entity = registry.create();

	Nc::Vector2f position = breakerCenterPosition + Nc::Vector2f::Right(80.0f);
    registry.emplace<Component::Transform>(entity, scene, position, SIZE, SIZE * 0.5f);
    registry.emplace<Component::Rectangle>(entity, WHITE);

    return entity;
}
