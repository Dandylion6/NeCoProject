#include "components/objects/comms/radar.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include <cmath>


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	constexpr float DEGRADATION_THRESHOLD = 20.0f; // The attraction percentage needed to start degrading stability.
	constexpr float PRECENTAGE_ADJUSTMENT = 1.0f / (100.0f - DEGRADATION_THRESHOLD);

	if (anomalyState.attractionPercentage < DEGRADATION_THRESHOLD) return;

	constexpr float DEGRADATION_FACTOR = 9.0f;
	constexpr float DEGRADATION_CURVE = 1.4f;

	auto view = registry.view<Component::RadarMachine>();
	for (auto [entity, machine] : view.each())
	{
		if (!machine.isActive) continue;

		float adjustedPercentage = (anomalyState.attractionPercentage - DEGRADATION_THRESHOLD) * PRECENTAGE_ADJUSTMENT;
		float curve = DEGRADATION_FACTOR * std::powf(DEGRADATION_CURVE, adjustedPercentage);
		float degredation = curve * 0.016f;
		machine.sability -= degredation * deltaTime;
	}
}