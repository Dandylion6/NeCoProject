#include "assemblers/entities/interactive/lever_entity.hpp"
#include "components/core/drag_action_component.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/objects/interactions/lever_component.hpp"
#include "components/objects/interactions/toggle_component.hpp"
#include "entt/entity/registry.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"
#include <utility>


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    const entt::entity entity, 
    Nc::Vector2f heightRange, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    assert(registry.any_of<Component::Transform>(entity) && "LeverEntity requires Transform!");

    registry.emplace<Component::DragAction>(entity);
    registry.emplace<Component::Toggle>(entity, On);

    Component::Lever& lever = registry.emplace<Component::Lever>(entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
    lever.currentHeight = heightRange.x;
    return { lever, entity };
}


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    const entt::entity entity, 
    Nc::Vector2f heightRange, 
    Texture2D&& handleTexture, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    registry.emplace<Component::Sprite>(entity, std::move(handleTexture));
    return Construct::LeverEntity(registry, entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
}


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    Component::Transform&& transform,
    float moveRange, 
    Texture2D&& handleTexture, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    const entt::entity entity = registry.create();

    Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);
    
    // The lever defaults to being on.
    transform.position.y = heightRange.x;
    registry.emplace<Component::Transform>(entity, std::move(transform));
    
    return Construct::LeverEntity(registry, entity, heightRange, std::move(handleTexture), std::move(movingAudioFile), std::move(switchedAudioFile));
}


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    Component::Transform&& transform,
    float moveRange, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
) noexcept
{
    const entt::entity entity = registry.create();

    Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);

    // The lever defaults to being on.
    transform.position.y = heightRange.x;

    registry.emplace<Component::Transform>(entity, std::move(transform));
    return Construct::LeverEntity(registry, entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
}