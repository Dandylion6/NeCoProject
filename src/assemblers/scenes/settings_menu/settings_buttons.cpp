#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/settings_menu/settings_buttons.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/ui/label_button_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/settings_tag.hpp"
#include "core/resource_store.hpp"
#include "core/save.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
#include <functional>
#include <utility>


LabelButton Construct::SettingsToMainButton(
    Settings& settings, Settings& pendingSettings, GameState& gameState, entt::registry& registry, ResourceStore& resourceStore
)
{
    Component::UiTransform transform = Component::UiTransform(
        Nc::Vector2f(0.3f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&registry, &gameState, &settings, &pendingSettings]()
    {
        SettingsMenu::Close(settings, pendingSettings, registry, gameState);
        MainMenu::Open(registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::Settings>(
        std::move(transform), "BACK TO MAIN", std::move(toMainMenu), registry, resourceStore
    );
}


LabelButton Construct::ApplySettingsButton(
    Settings& settings, Settings& pendingSettings, entt::registry& registry, ResourceStore& resourceStore
)
{
    Component::UiTransform transform = Component::UiTransform(
        Nc::Vector2f(0.7f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> applySettings = [&settings, &pendingSettings]()
    {
        Save::SaveSettings(pendingSettings);
        Settings::Apply(settings, pendingSettings);
    };

    return Construct::LabelButtonObject<Tag::Settings>(
        std::move(transform), "APPLY", std::move(applySettings), registry, resourceStore
    );
}