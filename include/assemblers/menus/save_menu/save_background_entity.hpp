#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
class ResourceStore;


namespace Construct
{
	const entt::entity SaveMenuBackgroundEntity(
		entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f windowSize
	);
}