#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"
class ResourceStore;


namespace Construct
{
	const entt::entity ProjectileEntity(
		entt::registry& registry, 
		ResourceStore& resourceStore, 
		Nc::Vector2f hitPosition
	);
}