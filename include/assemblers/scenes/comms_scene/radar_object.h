#pragma once 
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"


namespace Construct
{
	void RadarObject(entt::registry& registry);

	const entt::entity RadarPathEntity(entt::registry& registry);

	const entt::entity RadarArtilleryEntity(entt::registry& registry);

	const entt::entity RadarBlipEntity(
		entt::registry& registry, Nc::Vector2f position
	);
}