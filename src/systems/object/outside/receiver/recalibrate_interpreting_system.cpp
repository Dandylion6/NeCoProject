#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/outside/receiver/recalibrate_interpreting_system.hpp"


const std::string RecalibrateInterpretingSystem::COMMAND = "OPTSIG";


void RecalibrateInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	// The time it takes to recalibrate the radar machine in seconds.
	constexpr float RECALIBRATION_TIME = 10.0f;

	auto view = registry.view<Component::RadarMachine, Component::Machine>();
	for (auto [entity, radar, machine] : view.each())
	{
		if (radar.recalibrationTimeLeft > 0.0f)
		{
			// Already recalibrating
			// TODO: Add machine response
			continue;
		}

		// Turns off the machine to recalibrate.
		radar.recalibrationTimeLeft = RECALIBRATION_TIME;
		machine.isActive = false;
	}
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
			machine.isActive = true;
			float newStability = std::fminf(radar.stability + STABILITY_INCREASE, 100.0f);
			radar.stability = newStability;
			continue;
		}

		radar.recalibrationTimeLeft -= deltaTime;
	}
}