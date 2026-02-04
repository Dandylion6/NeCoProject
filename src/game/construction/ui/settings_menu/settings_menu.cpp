#include "game/construction/ui/settings_menu/settings_menu.hpp"

#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/construction/ui/settings_menu/entity/settings_header_entities.hpp"
#include "game/construction/ui/settings_menu/object/increment_setting_object.hpp"
#include "game/construction/ui/settings_menu/object/settings_background_object.hpp"
#include "game/construction/ui/settings_menu/object/settings_button_objects.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/state/settings.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"


void Structure::SettingsMenu::Build(
    const SceneContext& context,
    const Nc::Vector2f windowSize,
    Settings& live,
    Settings& pending
) noexcept
{
    constexpr Nc::Vector2f POSITION = Nc::Vector2f(0.3f, 0.3f);
    constexpr Nc::Vector2f MORSE_RANGE = Nc::Vector2f(0.1f, 0.4f);

    Entity::SettingsHeader::Create(context.registry);
    Entity::GameplaySettingsHeader::Create(context.registry);

    Object::SettingsToMainButton::Create(context);
    Object::ApplySettingsButton::Create(context);

    float* dotTimeValue = &pending.morseSettings.dotSeconds;
    const Object::IncrementSetting::Data data = Object::IncrementSetting::Create(
        context,
        "Morse code DOT duration",
        Component::UI::Increment(dotTimeValue, 0.02f, MORSE_RANGE, 2u),
        POSITION
    );

    for (const entt::entity entity : data.All())
    {
        context.registry.emplace<Tag::Settings>(entity);
        context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }

    Object::SettingsBackground::Create(context, windowSize);
    Close(context);
}


void Structure::SettingsMenu::Toggle(const SceneContext& context) noexcept
{
    const auto view = context.registry.view<const Tag::Settings, Component::Action::Toggle>();
    for (auto [settingsEntity, toggle] : view.each())
    {
        if (context.game.currentScene != NullScene)
            Toggle(context, Component::Action::Toggle::Next(toggle.state));
    }
}


void Structure::SettingsMenu::Open(const SceneContext& context) noexcept { Toggle(context, On); }


void Structure::SettingsMenu::Close(const SceneContext& context) noexcept { Toggle(context, Off); }


void Structure::SettingsMenu::Close(const SceneContext& context, const Settings& live, Settings& pending) noexcept
{
    // TODO: Add warning for unsaved changes.
    Close(context);
    pending = live; // Reverts back.
}


void Structure::SettingsMenu::Toggle(const SceneContext& context, const ToggleState state) noexcept
{
    const auto view = context.registry.view<const Tag::Settings, Component::UI::Transform>();
    for (auto [entity, transform] : view.each())
    {
        if (context.registry.any_of<Component::Action::Toggle>(entity))
        {
            Component::Action::Toggle& toggle = context.registry.get<Component::Action::Toggle>(entity);
            toggle.state = state;
        }
        transform.isVisible = state == On;
    }
    context.game.isPaused = state == On;
}
