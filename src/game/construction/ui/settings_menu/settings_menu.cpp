#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "assemblers/scenes/settings_menu/settings_buttons.hpp"
#include "assemblers/scenes/settings_menu/settings_headers.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/ui/increment_setting_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "components/ui/increment_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "components/ui/toggle_state_component.hpp"
#include "core/data/settings.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


void SettingsMenu::Build(
    Settings& settings, 
    Settings& pendingSettings, 
    GameState& gameState, 
    Nc::Vector2f windowSize, 
    entt::registry& registry, 
    ResourceStore& resourceStore
) 
{
    Construct::SettingsHeaderEntity(registry);
    Construct::GameplaySettingsHeaderEntity(registry);

    Construct::SettingsToMainButton(settings, pendingSettings, gameState, registry, resourceStore);
    Construct::ApplySettingsButton(settings, pendingSettings, registry, resourceStore);

    Construct::IncrementSettingObject<Tag::Settings, Tag::DontDestroyOnLoad>(
        Nc::Vector2f(0.3f, 0.3f),
        "Morse code DOT duration",
        Component::UiIncrement(&pendingSettings.morseSettings.dotTime, 0.02f, Nc::Vector2f(0.1f, 0.4f), 2u), 
        registry, 
        resourceStore
    );

    Construct::SettingsBackgroundEntity(registry, gameState, windowSize);

    SettingsMenu::Close(registry, gameState);
}


namespace SettingsMenu 
{
    static void Toggle(entt::registry& registry, GameState& gameState, bool active)
    {
        auto view = registry.view<const Tag::Settings, Component::UiTransform>();
        for (auto [entity, transform] : view.each())
        {
            if (registry.any_of<Component::ToggleState>(entity))
            {
                Component::ToggleState& toggle = registry.get<Component::ToggleState>(entity);
                toggle.isActive = active;
            }
            transform.isVisible = active;
        }
        gameState.isPaused = active;
    }
}


void SettingsMenu::Open(entt::registry& registry, GameState& gameState)
{
    SettingsMenu::Toggle(registry, gameState, true);
}


void SettingsMenu::Close(entt::registry& registry, GameState& gameState)
{
    SettingsMenu::Toggle(registry, gameState, false);
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
    auto view = registry.view<const Tag::Settings, Component::ToggleState>();
	for (auto [settingsEntity, toggle] : view.each())
	{
        if (gameState.currentScene != NullScene)
		    SettingsMenu::Toggle(registry, gameState, !toggle.isActive);
	}
}