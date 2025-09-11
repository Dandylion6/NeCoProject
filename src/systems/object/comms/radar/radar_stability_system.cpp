#include "components/objects/comms/radar.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include <cmath>


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	UpdateStability(registry, anomalyState, deltaTime);
}


void RadarStabilitySystem::UpdateStability(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	constexpr float DEGRADATION_FACTOR = 9.0f;
	constexpr float DEGRADATION_CURVE = 1.4f;

	auto view = registry.view<Component::RadarMachine>();
	for (auto [entity, machine] : view.each())
	{
		if (!machine.isActive) continue;

		if (anomalyState.attractionPercentage >= AnomalyState::DEGRADATION_THRESHOLD)
		{
			float adjustedPercentage = anomalyState.attractionPercentage - AnomalyState::DEGRADATION_THRESHOLD;
			float curve = DEGRADATION_FACTOR * std::powf(DEGRADATION_CURVE, adjustedPercentage * AnomalyState::PRECENTAGE_FACTOR);
			float degredation = curve * 0.016f;
			machine.sability -= degredation * deltaTime;
		}

		UpdateBlipStability(registry, machine);
	}
}


void RadarStabilitySystem::UpdateBlipStability(entt::registry& registry, Component::RadarMachine& machine)
{
	auto view = registry.view<Component::Blip>();
	for (auto [entity, blip] : view.each())
	{
		if (machine.sability >= 80.0f)
		{
			blip.coordState = Component::Blip::CoordTextState::Stable;
			continue;
		}

		
	}
}
