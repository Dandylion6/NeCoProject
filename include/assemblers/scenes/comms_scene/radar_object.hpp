#pragma once 
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
#include <cstdint>
class ResourceStore;


namespace Construct
{
	void RadarObject(entt::registry& registry, ResourceStore& resourceStore);
	const entt::entity RadarBlipEntity(
		entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f position, int16_t health = 10
	);
}