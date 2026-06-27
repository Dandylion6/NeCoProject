#include "game/system/scene/comms_scene/radar/radar_buttons_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radar/radar_screen_glitch_system.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"


void System::Radar::Buttons::Update(const SystemContext& context)
{
    const entt::entity powerButton = entt::get_single<Tag::Radar::Button>(context.registry);

    auto& click = context.registry.get<Component::Action::Click>(powerButton);
    auto& sprite = context.registry.get<Component::Sprite>(powerButton);
    auto& transform = context.registry.get<Component::Transform>(powerButton);

    if (click.justReleased)
    {
        sprite.scale = 1.0f;
        sprite.tint = Nc::RGBa(WHITE);
        transform.position -= BUTTON_OFFSET;
        return;
    }

    if (!click.justClicked) return;

    click.state = Component::Action::Click::Active;
    sprite.scale = 0.9f;
    sprite.tint = Nc::RGBa(220, 220, 220);
    transform.position += BUTTON_OFFSET;
}

void System::Radar::Buttons::Toggle(const SystemContext& context)
{
    const entt::entity radarEntity = entt::get_single<Component::Radar>(context.registry);
    auto& radar = context.registry.get<Component::Radar>(radarEntity);
    if (radar.isRecalibrating) return;

    auto& toggle = context.registry.get<Component::Action::Toggle>(radarEntity);
    if (toggle.state == On && radar.isTurningOff)
        toggle.state = Off;

    switch (toggle.state)
    {
    case Disabled: break;
    case Off:
        radar.isTurningOff = false;
        ScreenGlitch::StartGlitch(context, 0.6f);
        toggle.state = On;
        break;
    case On:
        ScreenGlitch::StartGlitch(context, 0.4f);
        radar.isTurningOff = true;
        radar.turningOffSecondsLeft = 0.16f;
        break;
    }
}
