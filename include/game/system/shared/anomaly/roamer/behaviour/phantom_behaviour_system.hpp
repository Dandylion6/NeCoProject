#pragma once
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;


namespace Nc
{
class ResourceStore;
};


namespace System::Anomaly::Roamer
{

class Phantom final
{
public:
	// ------ Functions ------
	static void Spawn(
		entt::registry& registry,
		const AnomalyState& anomalyState,
		entt::entity entity,
		Component::Anomaly::Roamer& roamer
	) noexcept;
	static void Update(
		entt::registry& registry,
		entt::entity entity,
		Component::Transform& transform,
		const Component::Anomaly::Roamer& roamer,
		float deltaTime
	) noexcept;
};

}

