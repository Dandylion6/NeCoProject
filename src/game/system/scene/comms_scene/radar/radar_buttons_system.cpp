#include "game/system/scene/comms_scene/radar/radar_buttons_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"


void System::Radar::Buttons::Update(const SystemContext& context)
{
    constexpr auto OFFSET = Nc::Vector2f::Scale(1.0f);

    const entt::entity powerButton = entt::get_single<Tag::Radar::Button>(context.registry);

    auto& click = context.registry.get<Component::Action::Click>(powerButton);
    auto& sprite = context.registry.get<Component::Sprite>(powerButton);
    auto& transform = context.registry.get<Component::Transform>(powerButton);

    if (click.justReleased)
    {
        sprite.scale = 1.0f;
        transform.position -= OFFSET;
        return;
    }

    if (!click.justClicked) return;

    // TODO: Add active/inactive visual state change and prevent spamming.
    click.state = Component::Action::Click::Active;
    sprite.scale = 0.9f;
    transform.position += OFFSET;


    const entt::entity radarEntity = entt::get_single<Component::Radar>(context.registry);
    const auto& radar = context.registry.get<Component::Radar>(radarEntity);
    if (radar.isRecalibrating) return;

    auto& toggle = context.registry.get<Component::Action::Toggle>(radarEntity);
    toggle.state = Component::Action::Toggle::Next(toggle.state);
}
