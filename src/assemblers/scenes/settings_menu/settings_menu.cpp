#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "assemblers/scenes/settings_menu/settings_headers.hpp"
#include "assemblers/ui/increment_setting_object.hpp"
#include "assemblers/ui/label_button_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/increment_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "components/ui/toggle_state_component.hpp"
#include "core/game_state.hpp"
#include "core/save.hpp"
#include "core/scene.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.hpp"
#include "utility/morse_code.hpp"
#include <functional>
#include <utility>


void SettingsMenu::Build(
    Settings& settings, GameState& gameState, Nc::Vector2f windowSize, entt::registry& registry, ResourceStore& resourceStore
) 
{
    Construct::SettingsHeaderEntity(registry);
    Construct::GameplaySettingsHeaderEntity(registry);

    Component::UiIncrement morseDotDuration = Component::UiIncrement(0.05f, 2u);
    morseDotDuration.onIncrement = [&morseSettings = settings.morseSettings](float increment)
    {
        morseSettings.dotTime += increment;
        morseSettings.dashTime = morseSettings.dotTime * 3.0f;
        morseSettings.errorMargin = morseSettings.dotTime * MorseCode::ERROR_MARGIN;
	    morseSettings.exitTime = morseSettings.dashTime + morseSettings.errorMargin + morseSettings.dotTime;
        return morseSettings.dotTime;
    };

    Component::UiTransform transform = Component::UiTransform(
        Nc::Vector2f(0.3f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&registry, &gameState]()
    {
        SettingsMenu::Close(registry, gameState);
        MainMenu::Open(registry, gameState);
    };

    Construct::LabelButtonObject<Tag::Settings>(
        std::move(transform), "BACK TO MAIN", std::move(toMainMenu), registry, resourceStore
    );

    Component::UiTransform transformb = Component::UiTransform(
        Nc::Vector2f(0.7f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> applySettings = [&settings]()
    {
        Save::SaveSettings(settings);
    };

    Construct::LabelButtonObject<Tag::Settings>(
        std::move(transformb), "APPLY", std::move(applySettings), registry, resourceStore
    );

    Construct::IncrementSettingObject<Tag::Settings>(
        Nc::Vector2f(0.3f, 0.3f), "Morse code DOT duration", std::move(morseDotDuration), registry, resourceStore
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


void SettingsMenu::Toggle(entt::registry& registry, GameState& gameState)
{
    auto view = registry.view<const Tag::Settings, Component::ToggleState>();
	for (auto [settingsEntity, toggle] : view.each())
	{
        if (toggle.isActive && gameState.currentScene == NullScene) return SettingsMenu::Close(registry, gameState);
        else if (gameState.currentScene == NullScene) return;
        return toggle.isActive ? SettingsMenu::Close(registry, gameState) : SettingsMenu::Open(registry, gameState);
	}
}