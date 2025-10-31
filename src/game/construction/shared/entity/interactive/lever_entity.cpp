#include "game/construction/shared/entity/mechanical/lever_entity.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "entt/entity/registry.hpp"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include <utility>


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    const entt::entity entity, 
    Nc::Vector2f heightRange, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
)
{
    assert(registry.any_of<Component::Transform>(entity) && "LeverEntity requires Transform!");

    registry.emplace<Component::DragAction>(entity);
    registry.emplace<Component::Toggle>(entity, Disabled);

    Component::Lever& lever = registry.emplace<Component::Lever>(entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
    return { lever, entity };
}


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    const entt::entity entity, 
    Nc::Vector2f heightRange, 
    Texture2D&& handleTexture, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
)
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
)
{
    const entt::entity entity = registry.create();

    Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);
    
    registry.emplace<Component::Transform>(entity, std::move(transform));
    
    return Construct::LeverEntity(registry, entity, heightRange, std::move(handleTexture), std::move(movingAudioFile), std::move(switchedAudioFile));
}


Assembled::LeverData Construct::LeverEntity(
    entt::registry& registry, 
    Component::Transform&& transform,
    float moveRange, 
    std::string&& movingAudioFile, 
    std::string&& switchedAudioFile
)
{
    const entt::entity entity = registry.create();

    Nc::Vector2f center = Nc::Bounds::CenterOf(Nc::Bounds(transform));
    Nc::Vector2f heightRange = Nc::Vector2f(center.y - moveRange, center.y + moveRange);

    registry.emplace<Component::Transform>(entity, std::move(transform));

    return Construct::LeverEntity(registry, entity, heightRange, std::move(movingAudioFile), std::move(switchedAudioFile));
}