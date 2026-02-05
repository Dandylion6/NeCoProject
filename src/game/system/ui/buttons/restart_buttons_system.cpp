#include "game/system/ui/buttons/restart_buttons_system.hpp"

#include "game/game.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "game/contexts/system_context.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"


void System::Restart::Buttons::Update(const SystemContext& context) noexcept
{
    const auto view = context.registry.view<Tag::RestartMenu, Component::Action::Click>();
    for (const auto& [entity, click] : view.each())
    {
        if (!click.justClicked) continue;

        switch (static_cast<Structure::RestartMenu::ButtonId>(click.id))
        {
        case Structure::RestartMenu::Restart:
            Restart(context);
            break;
        case Structure::RestartMenu::ToMainMenu:
            ToMainMenu(context);
            break;
        }
    }
}


void System::Restart::Buttons::ToMainMenu(const SystemContext& context) noexcept
{
    const auto sceneContext = SceneContext(context.registry, context.store, context.game);
    Structure::RestartMenu::Close(sceneContext);
    Structure::MainMenu::Open(sceneContext);
}


void System::Restart::Buttons::Restart(const SystemContext& context) noexcept
{
    context.gameEvents.shouldRestart = true;
    const auto sceneContext = SceneContext(context.registry, context.store, context.game);
    Structure::RestartMenu::Close(sceneContext);
}
