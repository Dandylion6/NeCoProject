#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"


void System::Anomaly::Roamer::Behaviour::Update(const SystemContext& context, AnomalyState& anomalyState) noexcept
{
	constexpr float PRESSURE_WEIGHT_SMOOTHING = 0.4f;

	float roamerPressureWeight = 0.0f;
	size_t totalRoamerCount = 0u;
	size_t roamerThreatCount = 0u;

	const auto view = context.registry.view<Component::Anomaly::Roamer>();
	for (auto [entity, roamer] : view.each())
	{
		++totalRoamerCount;
		roamerPressureWeight += GetPressureWeight(roamer.behaviour);
		if (roamer.behaviour != RoamerBehaviour::Phantom)
		{
			// Phantoms don't add to the threat value.
			++roamerThreatCount;
		}
	}

	anomalyState.roamerThreatCount = static_cast<uint32_t>(roamerThreatCount);
	anomalyState.totalRoamerCount = static_cast<uint8_t>(totalRoamerCount);

	anomalyState.roamerPressureWeight = Nc::Math::SmoothApproach(
		anomalyState.roamerPressureWeight,
		roamerPressureWeight,
		context.deltaTime,
		PRESSURE_WEIGHT_SMOOTHING
	);
};


Nc::Vector2f System::Anomaly::Roamer::Behaviour::GetTargetPosition(const RoamerTarget target) noexcept
{
	switch (target)
	{
	case RoamerTarget::Bunker: return BUNKER_POSITION;
	case RoamerTarget::Artillery: return ARTILLERY_POSITION;
	default: return Nc::Vector2f::Zero();
	}
}


float System::Anomaly::Roamer::Behaviour::GetPressureWeight(const RoamerBehaviour behaviour) noexcept
{
	constexpr float PHANTOM_PRESSURE = 0.2f;
	constexpr float PHASER_PRESSURE = 1.2f;
	constexpr float STRIDER_PRESSURE = 0.8f;

	switch (behaviour)
	{
	case RoamerBehaviour::Strider: return STRIDER_PRESSURE;
	case RoamerBehaviour::Phaser: return PHASER_PRESSURE;
	case RoamerBehaviour::Phantom: return PHANTOM_PRESSURE;
	default: return 0.0f;
	}
}
