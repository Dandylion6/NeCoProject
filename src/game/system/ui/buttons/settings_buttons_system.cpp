#include "game/system/ui/buttons/settings_buttons_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/ui/increment_button_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/contexts/system_context.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/settings.hpp"
#include "game/tag/ui/settings_tag.hpp"


void System::Settings::Buttons::Update(const SystemContext& context, ::Settings& live, ::Settings& pending) noexcept
{
    const auto view = context.registry.view<Tag::Settings, Component::Action::Click>();
    for (const auto& [entity, click] : view.each())
    {
        if (context.registry.any_of<Component::UI::IncrementButton>(entity)) continue;
        if (!click.justClicked) continue;

        switch (static_cast<Structure::SettingsMenu::ButtonId>(click.id))
        {
        case Structure::SettingsMenu::ToMainMenu:
            ToMainMenu(context, live, pending);
            break;
        case Structure::SettingsMenu::ApplySettings:
            ApplySettings(live, pending);
            break;
        }
    }
}


void System::Settings::Buttons::ToMainMenu(const SystemContext& context, const ::Settings& live, ::Settings& pending) noexcept
{
    const auto sceneContext = SceneContext(context.registry, context.store, context.game);

    Structure::SettingsMenu::Close(sceneContext, live, pending);
    Structure::MainMenu::Open(sceneContext);
}


void System::Settings::Buttons::ApplySettings(::Settings& live, const ::Settings& pending) noexcept
{
    live = pending;
    Save::SettingsToDisk(pending);
}
