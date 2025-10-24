#include "components/objects/comms/radar.hpp"
#include "components/objects/comms/radio_component.hpp"
#include "components/objects/machine.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/recalibrate_interpreting_system.hpp"
#include <cmath>
#include <string>
#include <utility>


const std::string RecalibrateInterpretingSystem::COMMAND = "OPTSIG";


void RecalibrateInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	auto view = registry.view<Component::Radar, Component::Machine>();
	for (auto [entity, radar, machine] : view.each())
	{
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
	auto view = registry.view<Component::Machine, Component::Radar>();
	for (auto [entity, machine, radar] : view.each())
	{
		if (!radar.isRecalibrating) continue;
		if (radar.recalibrationTimeLeft <= 0.0f)
		{
			RecalibrationCompleted(machine, radar);
			continue;
		}
		radar.recalibrationTimeLeft -= deltaTime;
	}
}


void RecalibrateInterpretingSystem::ConfirmRecalibrationCommand(
	entt::registry& registry, ResourceStore& resourceStore, Component::Receiver& receiver
)
{
	// TODO: Add response
    const std::string COORDINATE_RESPONSE = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(COORDINATE_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void RecalibrateInterpretingSystem::RecalibrationCompleted(Component::Machine& machine, Component::Radar& radar)
{
	// The amount of stability the radar machine regains after a successful recalibration.
	constexpr float STABILITY_INCREASE = 18.0f;

	// TODO: Reboot sequence
	radar.isRecalibrating = false;
	machine.isActive = true;
	float newStability = std::fminf(radar.stability + STABILITY_INCREASE, 100.0f);
	radar.stability = newStability;
}
