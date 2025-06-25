#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"


namespace Construct
{
	const entt::entity ProjectileEntity(
		entt::registry& registry, Nc::Vector2f hitPosition
	);
}