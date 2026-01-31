#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"

#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/construction/shared/entity/mechanical/lever_entity.hpp"
#include "game/state/scene.hpp"


Object::CircuitBreaker::Data Object::CircuitBreaker::Create(
    entt::registry& registry,
    const Component::Transform& transform,
    const entt::entity system
) noexcept
{
    constexpr char MOVE_AUDIO_PATH[] = "";
    constexpr char SWITCHED_AUDIO_PATH[] = "";

    const Scene scene = transform.boundScene;
    const auto [lever, entity] = Entity::Lever::Create(
        registry,
        transform,
        30.0f,
        MOVE_AUDIO_PATH,
        SWITCHED_AUDIO_PATH
    );

    const Nc::Vector2f centerPosition = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    entt::entity indicator = Indicator::Create(registry, scene, centerPosition);

    auto& breaker = registry.emplace<Component::Logic::Breaker>(entity, system, indicator);

    return { breaker, entity, indicator };
}


entt::entity Object::CircuitBreaker::Indicator::Create(
    entt::registry& registry,
    Scene scene,
    const Nc::Vector2f breakerCenterPosition
) noexcept
{
    // TODO: Add visuals.
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(20.0f);

    const entt::entity entity = registry.create();

    const Nc::Vector2f position = breakerCenterPosition + Nc::Vector2f::Right(80.0f);
    registry.emplace<Component::Transform>(entity, scene, position, SIZE, SIZE * 0.5f);
    registry.emplace<Component::Rectangle>(entity, Nc::RGBa(WHITE));

    return entity;
}
