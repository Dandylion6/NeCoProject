#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp" 
#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "core/data/interpolation.hpp"
#include "core/data/vector2.hpp"
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

	auto view = registry.view<Component::Transform, Component::Anomaly::Roamer>();
	for (auto [entity, transform, roamer] : view.each())
	{
		switch (roamer.behaviour)
		{
		case Component::Anomaly::Roamer::Strider:
			StriderBehaviourSystem::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += STRIDER_PRESSURE;
			break;
		case Component::Anomaly::Roamer::Phaser:
			PhaserBehaviourSystem::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += PHASER_PRESSURE;
			break;
		case Component::Anomaly::Roamer::Phantom:
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


Nc::Vector2f RoamerBehaviourSystem::GetTargetPosition(Component::Anomaly::Roamer::Target target)
{
	switch (target)
	{
	case Component::Anomaly::Roamer::Target::Bunker:
		return BUNKER_POSITION;
	case Component::Anomaly::Roamer::Target::Artillery:
		return ARTILLERY_POSITION;
	}
	return Nc::Vector2f::Zero();
}