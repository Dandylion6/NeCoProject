#pragma once 
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
#include <cstdint>
namespace Nc { class ResourceStore; };


namespace Construct
{
	void RadarObject(entt::registry& registry, Nc::ResourceStore& resourceStore);
	const entt::entity RadarBlipEntity(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Nc::Vector2f position, int16_t health = 10
	);
}