#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"

#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/lighting/point_light_component.hpp"
#include "game/component/core/rendering/lighting/spot_light_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/construction/shared/entity/mechanical/lever_entity.hpp"
#include "game/state/scene.hpp"


Object::CircuitBreaker::Data Object::CircuitBreaker::Create(
    entt::registry& registry,
    const Component::Transform& transform,
    const entt::entity system,
    const entt::entity base
) noexcept
{
    return Create(registry, transform, 30.0f, system, base);
}


Object::CircuitBreaker::Data Object::CircuitBreaker::Create(
    entt::registry& registry,
    const Component::Transform& transform,
    const float moveRange,
    entt::entity system,
    const entt::entity base
) noexcept
{
    constexpr char MOVE_AUDIO_PATH[] = "";
    constexpr char SWITCHED_AUDIO_PATH[] = "";

    const Scene scene = transform.boundScene;
    const auto [lever, entity] = Entity::Lever::Create(
        registry,
        transform,
        base,
        moveRange,
        MOVE_AUDIO_PATH,
        SWITCHED_AUDIO_PATH
    );

    const Nc::Vector2f centerPosition = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    entt::entity indicator = Indicator::Create(registry, scene, centerPosition);

    auto& breaker = registry.emplace<Component::Logic::Breaker>(entity, system, indicator);

    return { .breaker = breaker, .entity = entity, .indicator = indicator, .base = base };
}


entt::entity Object::CircuitBreaker::Indicator::Create(
    entt::registry& registry,
    Scene scene,
    const Nc::Vector2f breakerCenterPosition
) noexcept
{
    constexpr Nc::Vector2f SIZE = Nc::Vector2f::Scale(20.0f);

    const entt::entity entity = registry.create();

    const Nc::Vector2f position = breakerCenterPosition + Nc::Vector2f::Right(80.0f);
    registry.emplace<Component::Transform>(entity, scene, position, SIZE, SIZE * 0.5f);
    registry.emplace<Component::Sprite>(entity);
    registry.emplace<Component::Light::Point>(entity, Nc::RGBa(WHITE), 1.6f, 200.0f, 1.1f, 0.0f);

    return entity;
}
