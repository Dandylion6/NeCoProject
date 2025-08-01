#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "assemblers/scenes/settings_menu/settings_buttons.hpp"
#include "assemblers/scenes/settings_menu/settings_headers.hpp"
#include "assemblers/scenes/settings_menu/settings_headers.hpp"
#include "assemblers/ui/increment_setting_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/increment_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "components/ui/toggle_state_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "components/ui/toggle_state_component.hpp"
#include "core/game_state.hpp"
#include "core/save.hpp"
#include "core/scene.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


void SettingsMenu::Build(
    Settings& settings, Settings& pendingSettings, GameState& gameState, Nc::Vector2f windowSize, entt::registry& registry, ResourceStore& resourceStore
) 
{
    Construct::SettingsHeaderEntity(registry);
    Construct::GameplaySettingsHeaderEntity(registry);

    Construct::SettingsHeaderEntity(registry);
    Construct::GameplaySettingsHeaderEntity(registry);

    Construct::SettingsToMainButton(settings, pendingSettings, gameState, registry, resourceStore);
    Construct::ApplySettingsButton(settings, pendingSettings, registry, resourceStore);

    Construct::IncrementSettingObject<Tag::Settings>(
        Nc::Vector2f(0.3f, 0.3f),
        "Morse code DOT duration",
        Component::UiIncrement(&pendingSettings.morseSettings.dotTime, 0.05f, 2u), 
        registry, 
        resourceStore
    );

    Construct::SettingsBackgroundEntity(registry, gameState, windowSize);

    SettingsMenu::Close(registry, gameState);
}


void SettingsMenu::Open(entt::registry& registry, GameState& gameState)
{
    auto view = registry.view<const Tag::Settings, Component::UiTransform>();
	for (auto [entity, transform] : view.each())
	{
        if (registry.any_of<Component::ToggleState>(entity))
        {
            Component::ToggleState& toggle = registry.get<Component::ToggleState>(entity);
            toggle.isActive = true;
            gameState.isPaused = true;
        }
        transform.isVisible = true;
	}
}


void SettingsMenu::Close(entt::registry& registry, GameState& gameState)
{
    auto view = registry.view<const Tag::Settings, Component::UiTransform>();
	for (auto [entity, transform] : view.each())
	{
        if (registry.any_of<Component::ToggleState>(entity))
        {
            Component::ToggleState& toggle = registry.get<Component::ToggleState>(entity);
            toggle.isActive = false;
            gameState.isPaused = false;
        }
        transform.isVisible = false;
	}
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
        if (gameState.currentScene == NullScene) return;
        return toggle.isActive ? SettingsMenu::Close(registry, gameState) : SettingsMenu::Open(registry, gameState);
	}
}