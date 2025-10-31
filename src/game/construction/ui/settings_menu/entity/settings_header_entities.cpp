#include "game/construction/ui/settings_menu/entity/settings_header_entities.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/utility/color_palette.hpp"


const entt::entity Construct::SettingsHeaderEntity(entt::registry& registry)
{
    const entt::entity entity = registry.create();

    registry.emplace<Tag::DontDestroyOnLoad>(entity);
    registry.emplace<Tag::Settings>(entity);
    registry.emplace<Component::UI::Transform>(entity, Nc::Vector2f(0.5f, 0.1f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(entity, "SETTINGS", Palette::RADAR_COLOR, WDXL, FontSize::Huge, Alignment::Center);

    return entity;
}


const entt::entity Construct::GameplaySettingsHeaderEntity(entt::registry& registry)
{
    const entt::entity entity = registry.create();

    registry.emplace<Tag::DontDestroyOnLoad>(entity);
    registry.emplace<Tag::Settings>(entity);
    registry.emplace<Component::UI::Transform>(entity, Nc::Vector2f(0.3f, 0.25f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(entity, "GAMEPLAY", Palette::RADAR_COLOR, WDXL, FontSize::Large, Alignment::Left);

    return entity;
}