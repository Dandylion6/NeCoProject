#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/construction/shared/entity/mechanical/lever_entity.hpp"
#include "raylib.h"
#include <cassert>
#include <string>
#include <utility>


Entity::Lever::Data Entity::Lever::Create(
    entt::registry& registry,
    const entt::entity entity,
    Nc::Vector2f heightRange, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
#ifdef DEBUG_BUILD
    assert(registry.any_of<Component::Transform>(entity) && "LeverEntity requires Transform!");
#endif

    registry.emplace<Component::Action::Drag>(entity);
    registry.emplace<Component::Action::Toggle>(entity, Disabled);

    auto& lever = registry.emplace<Component::Logic::Lever>(
        entity,
        heightRange,
        std::move(movingAudioFile),
        std::move(switchedAudioFile)
    );

    lever.currentHeight = heightRange.x;
    return { lever, entity };
}


Entity::Lever::Data Entity::Lever::Create(
    entt::registry& registry,
    const entt::entity entity,
    const Nc::Vector2f heightRange,
    const Texture2D& handleTexture,
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    registry.emplace<Component::Sprite>(entity, handleTexture);
    return Create(registry, entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
}


Entity::Lever::Data Entity::Lever::Create(
    entt::registry& registry, 
    const Component::Transform& transform,
    const float moveRange,
    const Texture2D& handleTexture,
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    const entt::entity entity = registry.create();

    const Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    const Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);
    
    // The lever defaults to being on.
    auto& leverTransform = registry.emplace<Component::Transform>(entity, transform);
    leverTransform.position.y = heightRange.x;

    return Create(registry, entity, heightRange, handleTexture, std::move(movingAudioFile), std::move(switchedAudioFile));
}


Entity::Lever::Data Entity::Lever::Create(
    entt::registry& registry, 
    const Component::Transform& transform,
    const float moveRange,
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    const entt::entity entity = registry.create();

    const Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    const Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);

    // The lever defaults to being on.
    auto& leverTransform = registry.emplace<Component::Transform>(entity, transform);
    leverTransform.position.y = heightRange.x;

    return Create(registry, entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
}