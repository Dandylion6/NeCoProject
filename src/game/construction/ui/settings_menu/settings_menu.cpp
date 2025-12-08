#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/construction/ui/settings_menu/entity/settings_background_entity.hpp"
#include "game/construction/ui/settings_menu/entity/settings_header_entities.hpp"
#include "game/construction/ui/settings_menu/object/increment_setting_object.hpp"
#include "game/construction/ui/settings_menu/object/settings_button_objects.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/state/settings.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"


void SettingsMenu::Build(
    Settings& settings, 
    Settings& pendingSettings, 
    GameState& gameState, 
    Nc::Vector2f windowSize, 
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore
) 
{
    Construct::SettingsHeaderEntity(registry);
    Construct::GameplaySettingsHeaderEntity(registry);

    Construct::SettingsToMainButton(settings, pendingSettings, gameState, registry, resourceStore);
    Construct::ApplySettingsButton(settings, pendingSettings, registry, resourceStore);

    Construct::IncrementSettingObject<Tag::Settings, Tag::DontDestroyOnLoad>(
        Nc::Vector2f(0.3f, 0.3f),
        "Morse code DOT duration",
        Component::UI::Increment(&pendingSettings.morseSettings.dotTime, 0.02f, Nc::Vector2f(0.1f, 0.4f), 2u), 
        registry, 
        resourceStore
    );

    Construct::SettingsBackgroundEntity(registry, gameState, windowSize);

    SettingsMenu::Close(registry, gameState);
}


namespace SettingsMenu 
{
    static void Toggle(entt::registry& registry, GameState& gameState, ToggleState state)
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
}


void SettingsMenu::Open(entt::registry& registry, GameState& gameState)
{
    SettingsMenu::Toggle(registry, gameState, On);
}


void SettingsMenu::Close(entt::registry& registry, GameState& gameState)
{
    SettingsMenu::Toggle(registry, gameState, Off);
}


void SettingsMenu::Close(Settings& settings, Settings& pendingSettings, entt::registry& registry, GameState& gameState)
{
    if (settings == pendingSettings) return SettingsMenu::Close(registry, gameState); 
    // TODO: Add warning for unsaved changes.
    SettingsMenu::Close(registry, gameState);
    pendingSettings = settings;
}


void SettingsMenu::Toggle(entt::registry& registry, GameState& gameState)
{
    auto view = registry.view<const Tag::Settings, Component::Action::Toggle>();
	for (auto [settingsEntity, toggle] : view.each())
	{
        if (gameState.currentScene != NullScene)
		    SettingsMenu::Toggle(registry, gameState, Component::Action::Toggle::Next(toggle.state));
	}
}