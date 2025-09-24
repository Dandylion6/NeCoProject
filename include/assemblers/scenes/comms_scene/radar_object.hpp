#pragma once 
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
#include <cstdint>
class ResourceStore;


namespace Construct
{
	void RadarObject(entt::registry& registry, ResourceStore& resourceStore);

	const entt::entity RadarPathEntity(entt::registry& registry);

	const entt::entity RadarArtilleryEntity(entt::registry& registry);

	const entt::entity RadarBlipEntity(
		entt::registry& registry, Nc::Vector2f position, int16_t health = 10
	);

	const entt::entity RadarErrorWarningEntity(entt::registry& registry);

	const entt::entity RadarRecalibrationTextEntity(entt::registry& registry);

	const entt::entity RadarPowerButtonEntity(entt::registry& registry);
}