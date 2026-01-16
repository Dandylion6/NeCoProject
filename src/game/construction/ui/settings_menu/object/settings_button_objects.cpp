#include "game/construction/ui/settings_menu/object/settings_button_objects.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/settings.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"
#include <functional>
#include <utility>


void Object::SettingsToMainButton::Create(SceneContext context, Settings& settings, Settings& pendingSettings) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.3f, 0.8f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> toMainMenu = [context, &settings, &pendingSettings]()
    {
        Structure::SettingsMenu::Close(context, settings, pendingSettings);
        Structure::MainMenu::Open(context);
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Data data = Object::LabelButton::Create(
        context,
        std::move(transform), 
        "BACK TO MAIN", 
        std::move(toMainMenu)
    );

    for (entt::entity entity : data.All())
    {
        context.registry.emplace<Tag::Settings>(entity);
        context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }
}


void Object::ApplySettingsButton::Create(SceneContext context, Settings& settings, Settings& pendingSettings) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.7f, 0.8f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> applySettings = [&settings, &pendingSettings]()
    {
        Save::SettingsToDisk(pendingSettings);
        settings = pendingSettings;
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Data data = Object::LabelButton::Create(
        context,
        std::move(transform), 
        "APPLY", 
        std::move(applySettings)
    );

    for (entt::entity entity : data.All())
    {
        context.registry.emplace<Tag::Settings>(entity);
        context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
    }
}
