#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/outside/receiver/recalibrate_interpreting_system.hpp"
#include "systems/object/comms/radio_sound_system.hpp"
#include "raylib.h"


const std::string RecalibrateInterpretingSystem::COMMAND = "OPTSIG";


void RecalibrateInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	auto view = registry.view<Component::RadarMachine, Component::Machine>();
	for (auto [entity, radar, machine] : view.each())
	{
		if (radar.recalibrationTimeLeft > 0.0f)
		{
			// Already recalibrating
			// TODO: Add machine feedback
			continue;
		}

		// Starts recalibration.
		radar.recalibrationTimeLeft = Component::RadarMachine::RECALIBRATION_TIME;
	}

	ConfirmRecalibrationCommand(registry, resourceStore, receiver);
};


void RecalibrateInterpretingSystem::Update(entt::registry& registry, float deltaTime)
{
	// The amount of stability the radar machine regains after a successful recalibration.
	constexpr float STABILITY_INCREASE = 33.33f;

	auto view = registry.view<Component::Machine, Component::RadarMachine>();
	for (auto [entity, machine, radar] : view.each())
	{
		// Restart the radar machine if it's done recalibrating.
		if (radar.recalibrationTimeLeft <= 0.0f)
		{
			// TODO: Reboot sequence
			float newStability = std::fminf(radar.stability + STABILITY_INCREASE, 100.0f);
			radar.stability = newStability;
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