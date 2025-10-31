#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/object/settings_button_objects.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/settings.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
#include <functional>
#include <utility>


LabelButton Construct::SettingsToMainButton(
    Settings& settings, Settings& pendingSettings, GameState& gameState, entt::registry& registry, ResourceStore& resourceStore
)
{
    Component::UI::Transform transform = Component::UI::Transform(
        Nc::Vector2f(0.3f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&registry, &gameState, &settings, &pendingSettings]()
    {
        SettingsMenu::Close(settings, pendingSettings, registry, gameState);
        MainMenu::Open(registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::Settings, Tag::DontDestroyOnLoad>(
        std::move(transform), "BACK TO MAIN", std::move(toMainMenu), registry, resourceStore
    );
}


LabelButton Construct::ApplySettingsButton(
    Settings& settings, Settings& pendingSettings, entt::registry& registry, ResourceStore& resourceStore
)
{
    Component::UI::Transform transform = Component::UI::Transform(
        Nc::Vector2f(0.7f, 0.8f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> applySettings = [&settings, &pendingSettings]()
    {
        Save::SaveSettings(pendingSettings);
        Settings::Apply(settings, pendingSettings);
    };

    return Construct::LabelButtonObject<Tag::Settings, Tag::DontDestroyOnLoad>(
        std::move(transform), "APPLY", std::move(applySettings), registry, resourceStore
    );
}