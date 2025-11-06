#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Construct
{
	const entt::entity ProjectileEntity(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore, 
		Nc::Vector2f hitPosition
	);
}