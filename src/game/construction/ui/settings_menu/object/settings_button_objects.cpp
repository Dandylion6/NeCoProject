#include "game/construction/ui/settings_menu/object/settings_button_objects.hpp"

#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"


void Object::SettingsToMainButton::Create(const SceneContext& context) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.3f, 0.8f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    auto transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    const LabelButton::Data data = LabelButton::Create(
        context,
        transform,
        "BACK TO MAIN",
        Structure::SettingsMenu::ToMainMenu
    );

    for (const entt::entity entity : data.All())
    {
        context.registry.emplace<Tag::Settings>(entity);
        context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }
}


void Object::ApplySettingsButton::Create(const SceneContext& context) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.7f, 0.8f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    auto transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    const LabelButton::Data data = LabelButton::Create(
        context,
        transform,
        "APPLY",
        Structure::SettingsMenu::ApplySettings
    );

    for (const entt::entity entity : data.All())
    {
        context.registry.emplace<Tag::Settings>(entity);
        context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }
}
