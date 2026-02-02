#include "game/system/scene/comms_scene/radar/radar_buttons_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"


void System::Radar::Buttons::Update(const SystemContext& context)
{
    const entt::entity powerButton = entt::get_single<Tag::Radar::Button>(context.registry);

    auto& click = context.registry.get<Component::Action::Click>(powerButton);
    if (!click.justClicked) return;

    // TODO: Add active/inactive visual state change and prevent spamming.
    click.state = Component::Action::Click::Active;

    const entt::entity radarEntity = entt::get_single<Component::Radar>(context.registry);
    const auto& radar = context.registry.get<Component::Radar>(radarEntity);
    if (radar.isRecalibrating) return;

    auto& toggle = context.registry.get<Component::Action::Toggle>(radarEntity);
    toggle.state = Component::Action::Toggle::Next(toggle.state);
}
