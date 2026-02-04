#include "game/system/ui/interactive/settings_input_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/contexts/system_context.hpp"
#include "game/tag/ui/settings_tag.hpp"


void System::Settings::Input::Update(const SystemContext& context)
{
    const entt::entity entity = entt::get_single<Tag::Settings, Component::Action::Input>(context.registry);
    const auto& input = context.registry.get<Component::Action::Input>(entity);

    if (input.state == Component::Action::Input::Released)
    {
        const auto sceneContext = SceneContext(context.registry, context.store, context.game);
        Structure::SettingsMenu::Toggle(sceneContext);
    }
}
