#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/scene/comms_scene/radio/radio_sound_system.hpp"
#include "game/system/scene/outside_scene/receiver/recalibrate_interpreting_system.hpp"
#include <cmath>
#include <string>
#include <utility>


const std::string RecalibrateInterpretingSystem::COMMAND = "OPTSIG";


void RecalibrateInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Receiver& receiver, const std::string& message
)
{
	auto view = registry.view<Component::Radar, Component::Toggle>();
	for (auto [entity, radar, toggle] : view.each())
	{
		if (toggle.state == Disabled)
		{
			// TODO: Add failed recalibaration response.
			continue;
		}

		if (radar.isRecalibrating)
		{
			// Already recalibrating
			// TODO: Add machine feedback
			continue;
		}

		// Starts recalibration.
		radar.recalibrationTimeLeft = Component::Radar::RECALIBRATION_TIME;
		radar.isRecalibrating = true;
	}

	// TODO: Add confirmation response
	receiver.message.clear();
	//ConfirmRecalibrationCommand(registry, resourceStore, receiver);
};


void RecalibrateInterpretingSystem::Update(entt::registry& registry, float deltaTime)
{
	auto view = registry.view<Component::Radar, Component::Toggle>();
	for (auto [entity, radar, toggle] : view.each())
	{
		if (!radar.isRecalibrating) continue;
		if (radar.recalibrationTimeLeft <= 0.0f)
		{
			RecalibrationCompleted(radar, toggle);
			continue;
		}
		radar.recalibrationTimeLeft -= deltaTime;
	}
}


void RecalibrateInterpretingSystem::ConfirmRecalibrationCommand(
	entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Receiver& receiver
)
{
	// TODO: Add response
    const std::string COORDINATE_RESPONSE = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(COORDINATE_RESPONSE));
	RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void RecalibrateInterpretingSystem::RecalibrationCompleted(Component::Radar& radar, Component::Toggle& toggle)
{
	// The amount of stability the radar machine regains after a successful recalibration.
	constexpr float STABILITY_INCREASE = 18.0f;

	// TODO: Reboot sequence
	radar.isRecalibrating = false;
	toggle.state = On;
	float newStability = std::fminf(radar.stability + STABILITY_INCREASE, 100.0f);
	radar.stability = newStability;
}
