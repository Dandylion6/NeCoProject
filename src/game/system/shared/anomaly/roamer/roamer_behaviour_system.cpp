#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"


void System::Anomaly::Roamer::Behaviour::Update(
	entt::registry& registry,
	AnomalyState& anomalyState,
	const float deltaTime
)
{
	// TODO: Make each behavior update itself.
	constexpr float PRESSURE_WEIGHT_SMOOTHING = 0.4f;

	float roamerPressureWeight = 0.0f;
	uint8_t totalRoamerCount = 0u;
	uint8_t roamerThreatCount = 0u;

	const auto view = registry.view<Component::Transform, Component::Anomaly::Roamer>();
	for (auto [entity, transform, roamer] : view.each())
	{
		constexpr float PHANTOM_PRESSURE = 0.2f;
		constexpr float PHASER_PRESSURE = 1.2f;
		constexpr float STRIDER_PRESSURE = 0.8f;

		switch (roamer.behaviour)
		{
		case RoamerBehaviour::Strider:
			Strider::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += STRIDER_PRESSURE;
			break;
		case RoamerBehaviour::Phaser:
			Phaser::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += PHASER_PRESSURE;
			break;
		case RoamerBehaviour::Phantom:
			Phantom::Update(registry, entity, transform, roamer, deltaTime);
			roamerPressureWeight += PHANTOM_PRESSURE;
			--roamerThreatCount; // Doesn't count as a threat.
			break;
		default: break;
		}
		++roamerThreatCount;
		++totalRoamerCount;
	}

	anomalyState.roamerPressureWeight = roamerThreatCount;
	anomalyState.totalRoamerCount = totalRoamerCount;
	anomalyState.roamerPressureWeight = Nc::Math::SmoothApproach(
		anomalyState.roamerPressureWeight,
		roamerPressureWeight,
		deltaTime,
		PRESSURE_WEIGHT_SMOOTHING
	);
};


Nc::Vector2f System::Anomaly::Roamer::Behaviour::GetTargetPosition(const RoamerTarget target)
{
	switch (target)
	{
	case RoamerTarget::Bunker: return BUNKER_POSITION;
	case RoamerTarget::Artillery: return ARTILLERY_POSITION;
	default: return Nc::Vector2f::Zero();
	}
}
