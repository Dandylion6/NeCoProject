#include "game/system/ui/buttons/menu_buttons_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_events.hpp"
#include "game/tag/ui/main_menu_tag.hpp"


void System::Menu::Buttons::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Tag::MainMenu, Component::Action::Click>();
    for (const auto& [entity, click] : view.each())
    {
        if (!click.justClicked) continue;

        switch (static_cast<Structure::MainMenu::ButtonId>(click.id))
        {
        case Structure::MainMenu::Play:
            Play(context);
            break;
        case Structure::MainMenu::Settings:
            ToSettings(context);
            break;
        case Structure::MainMenu::Exit:
            Exit(context);
            break;
        }
    }
}


void System::Menu::Buttons::Play(const SystemContext& context)
{
    context.gameEvents.shouldLoad = true;
    const auto sceneContext = SceneContext(context.registry, context.store, context.game);
    Structure::MainMenu::Close(sceneContext);
}


void System::Menu::Buttons::ToSettings(const SystemContext& context)
{
    const auto sceneContext = SceneContext(context.registry, context.store, context.game);
    Structure::MainMenu::Close(sceneContext);
    Structure::SettingsMenu::Open(sceneContext);
}


void System::Menu::Buttons::Exit(const SystemContext& context)
{
    context.gameEvents.shouldExit = true;
}
