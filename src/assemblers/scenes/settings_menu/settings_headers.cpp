#include "assemblers/scenes/settings_menu/settings_headers.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color_palette.hpp"


const entt::entity Construct::SettingsHeaderEntity(entt::registry& registry)
{
    const entt::entity entity = registry.create();

    registry.emplace<Tag::Settings>(entity);
    registry.emplace<Component::UiTransform>(entity, Nc::Vector2f(0.5f, 0.1f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(entity, "SETTINGS", RADAR_COLOR, WDXL, FontSize::Huge, Alignment::Center);

    return entity;
}


const entt::entity Construct::GameplaySettingsHeaderEntity(entt::registry& registry)
{
    const entt::entity entity = registry.create();

    registry.emplace<Tag::Settings>(entity);
    registry.emplace<Component::UiTransform>(entity, Nc::Vector2f(0.3f, 0.25f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(entity, "GAMEPLAY", RADAR_COLOR, WDXL, FontSize::Large, Alignment::Left);

    return entity;
}