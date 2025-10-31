#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
class ResourceStore;


namespace Construct
{
	const entt::entity ProjectileEntity(
		entt::registry& registry, 
		ResourceStore& resourceStore, 
		Nc::Vector2f hitPosition
	);
}