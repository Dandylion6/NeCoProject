#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/transform_component.hpp"
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
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore,
    Settings& settings, 
    Settings& pendingSettings, 
    GameState& gameState, 
    Nc::Vector2f windowSize
) noexcept
{
    constexpr Nc::Vector2f POSITION = Nc::Vector2f(0.3f, 0.3f);
    constexpr Nc::Vector2f MORSE_RANGE = Nc::Vector2f(0.1f, 0.4f);
    
    Entity::SettingsHeader::Create(registry);
    Entity::GameplaySettingsHeader::Create(registry);

    Object::SettingsToMainButton::Create(registry, resourceStore, settings, pendingSettings, gameState);
    Object::ApplySettingsButton::Create(registry, resourceStore, settings, pendingSettings);

    float* dotTimeValue = &pendingSettings.morseSettings.dotTime;
    Component::UI::Increment increment = Component::UI::Increment(dotTimeValue, 0.02f, MORSE_RANGE, 2u);

    const Object::IncrementSetting::Data data = Object::IncrementSetting::Create(
        registry, 
        resourceStore, 
        "Morse code DOT duration", 
        std::move(increment), 
        POSITION
    );

    for (entt::entity entity : data.All())
    {
        registry.emplace<Tag::Settings>(entity);
        registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }

    Object::SettingsBackground::Create(registry, gameState, windowSize);

    SettingsMenu::Close(registry, gameState);
}


void Structure::SettingsMenu::Toggle(entt::registry& registry, GameState& gameState) noexcept
{
    auto view = registry.view<const Tag::Settings, Component::Action::Toggle>();
	for (auto [settingsEntity, toggle] : view.each())
	{
        if (gameState.currentScene != NullScene)
		    SettingsMenu::Toggle(registry, gameState, Component::Action::Toggle::Next(toggle.state));
	}
}


void Structure::SettingsMenu::Open(entt::registry& registry, GameState& gameState) noexcept
{
    SettingsMenu::Toggle(registry, gameState, On);
}


void Structure::SettingsMenu::Close(entt::registry& registry, GameState& gameState) noexcept
{
    SettingsMenu::Toggle(registry, gameState, Off);
}


void Structure::SettingsMenu::Close(
    entt::registry& registry, 
    GameState& gameState, 
    Settings& settings, 
    Settings& pendingSettings
) noexcept
{
    if (settings == pendingSettings) return SettingsMenu::Close(registry, gameState); 
    // TODO: Add warning for unsaved changes.
    SettingsMenu::Close(registry, gameState);
    pendingSettings = settings;
}


void Structure::SettingsMenu::Toggle(entt::registry& registry, GameState& gameState, ToggleState state) noexcept
{
    auto view = registry.view<const Tag::Settings, Component::UI::Transform>();
    for (auto [entity, transform] : view.each())
    {
        if (registry.any_of<Component::Action::Toggle>(entity))
        {
            Component::Action::Toggle& toggle = registry.get<Component::Action::Toggle>(entity);
            toggle.state = state;
        }
        transform.isVisible = state == On;
    }
    gameState.isPaused = state == On;
}