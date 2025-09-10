#include "components/objects/comms/radar.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	if (anomalyState.intensity <= 1u) return;

	constexpr float DEGRADATION_FACTOR = 9.0f; // It takes this amount of minutes to go from 100 to 0 at an intensity level of 2.
	constexpr float PRECOMP_CURVES[AnomalyState::MAX_INTENSITY] = {
		 DEGRADATION_FACTOR , DEGRADATION_FACTOR * 1.4f, DEGRADATION_FACTOR * 1.96f
	};

	auto view = registry.view<Component::RadarMachine>();
	for (auto [entity, machine] : view.each())
	{
		float curve = PRECOMP_CURVES[anomalyState.intensity];
		float degredation = curve * 0.016f;
		machine.sability -= degredation * deltaTime;
	}
}