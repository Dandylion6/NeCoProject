#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/state/anomaly_state.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer/phantom_behaviour_system.hpp"
#include "systems/anomaly/roamer/phaser_behaviour_system.hpp" 
#include "systems/anomaly/roamer/strider_behaviour_system.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


void RoamerBehaviourSystem::Update(
	entt::registry& registry, AnomalyState& anomalyState, float deltaTime
)
{
	constexpr float STRIDER_PRESSURE = 0.8f;
	constexpr float PHASER_PRESSURE = 1.2f;
	constexpr float PHANTOM_PRESSURE = 0.2f;
	constexpr float PRESSURE_WEIGHT_SMOOTHING = 0.4f;

	float roamerPressureWeight = 0.0f;
	uint8_t totalRoamerCount = 0u;
	uint8_t roamerThreatCount = 0u;

	auto view = registry.view<Component::Transform, Component::AnomalyRoamer>();
	for (auto [entity, transform, roamer] : view.each())
	{
		switch (roamer.behaviour)
		{
		case Component::AnomalyRoamer::Strider:
			StriderBehaviourSystem::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += STRIDER_PRESSURE;
			break;
		case Component::AnomalyRoamer::Phaser:
			PhaserBehaviourSystem::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += PHASER_PRESSURE;
			break;
		case Component::AnomalyRoamer::Phantom:
			PhantomBehaviourSystem::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += PHANTOM_PRESSURE;
			--roamerThreatCount; // Doesn't count as a threat.
			break;
		default:
			break;
		}
		++roamerThreatCount;
		++totalRoamerCount;
	}
	anomalyState.roamerPressureWeight = roamerThreatCount;
	anomalyState.totalRoamerCount = totalRoamerCount;
	anomalyState.roamerPressureWeight = Math::SmoothApproach(
		anomalyState.roamerPressureWeight, roamerPressureWeight, deltaTime, PRESSURE_WEIGHT_SMOOTHING
	);
};


Nc::Vector2f RoamerBehaviourSystem::GetTargetPosition(Component::AnomalyRoamer::Target target)
{
	switch (target)
	{
	case Component::AnomalyRoamer::Target::Bunker:
		return BUNKER_POSITION;
	case Component::AnomalyRoamer::Target::Artillery:
		return ARTILLERY_POSITION;
	}
	return Nc::Vector2f::Zero();
}