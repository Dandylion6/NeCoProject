#include "game/system/scene/outside_scene/receiver/interpret_recalibration_system.hpp"

#include <cmath>
#include <string>

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radar/radar_screen_glitch_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"


void System::Receiver::Interpret::Recalibration::HandleMessage(
    const SystemContext& context,
    Component::Receiver& receiver
)
{
    const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
    auto& radar = context.registry.get<Component::Radar>(entity);
    const auto& toggle = context.registry.get<Component::Action::Toggle>(entity);

    if (toggle.state == Disabled)
    {
        // TODO: Add failed recalibration response.
        return;;
    }

    if (radar.isRecalibrating)
    {
        // Already recalibrating
        // TODO: Add machine feedback
        return;;
    }

    // Starts recalibration.
    radar.recalibrationTimeLeft = Component::Radar::RECALIBRATION_TIME;
    radar.isRecalibrating = true;

    // TODO: Add confirmation response
    receiver.message.clear();
    //ConfirmRecalibrationCommand(registry, resourceStore, receiver);
};


void System::Receiver::Interpret::Recalibration::Update(const SystemContext& context)
{
    const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
    auto& radar = context.registry.get<Component::Radar>(entity);
    auto& toggle = context.registry.get<Component::Action::Toggle>(entity);

    if (!radar.isRecalibrating) return;

    if (radar.recalibrationTimeLeft <= 0.0f)
        return Completed(context, radar, toggle);

    radar.recalibrationTimeLeft -= context.deltaTime;
}


void System::Receiver::Interpret::Recalibration::ConfirmCommand(
    const SystemContext& context,
    Component::Receiver& receiver
)
{
    // TODO: Add response
    constexpr char COORDINATE_RESPONSE[] = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

    const Sound& response = context.store.CreateSoundHandle(COORDINATE_RESPONSE);
    Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Medium);

    receiver.message.clear();
}


void System::Receiver::Interpret::Recalibration::Completed(
    const SystemContext& context,
    Component::Radar& radar,
    Component::Action::Toggle& toggle
)
{
    // The amount of stability the radar machine regains after a successful recalibration.
    constexpr float STABILITY_INCREASE = 18.0f;

    // TODO: Reboot sequence
    radar.isRecalibrating = false;
    toggle.state = On;
    const float newStability = std::fminf(radar.stability + STABILITY_INCREASE, 100.0f);
    radar.stability = newStability;

    Radar::ScreenGlitch::StartGlitch(context, 0.8f);
}
