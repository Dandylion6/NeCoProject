#include "core/data/font.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/settings_menu/entity/settings_header_entities.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"
#include "game/utility/color_palette.hpp"


entt::entity Entity::SettingsHeader::Create(entt::registry& registry) noexcept
{
    constexpr auto ANCHOR = Nc::Vector2f(0.5f, 0.1f);

    entt::entity entity = registry.create();

    registry.emplace<Tag::DontDestroyOnLoad>(entity);
    registry.emplace<Tag::Settings>(entity);

    registry.emplace<Component::UI::Transform>(entity, ANCHOR, Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(
        entity, 
        "SETTINGS", 
        Palette::RADAR_COLOR, 
        Nc::Font::WDXL, 
        Nc::Font::Size::Huge, 
        Alignment::Center
    );

    return entity;
}


entt::entity Entity::GameplaySettingsHeader::Create(entt::registry& registry) noexcept
{
    constexpr auto ANCHOR = Nc::Vector2f(0.3f, 0.25f);

    entt::entity entity = registry.create();

    registry.emplace<Tag::DontDestroyOnLoad>(entity);
    registry.emplace<Tag::Settings>(entity);

    registry.emplace<Component::UI::Transform>(entity, ANCHOR, Nc::Vector2f::Zero(), 2);
    registry.emplace<Component::Text>(
        entity, 
        "GAMEPLAY", 
        Palette::RADAR_COLOR, 
        Nc::Font::WDXL, 
        Nc::Font::Size::Large, 
        Alignment::Left
    );

    return entity;
}
