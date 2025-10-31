#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct GameState;
class ResourceStore;


namespace Construct
{
	const entt::entity RestartMenuBackgroundEntity(
		entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f windowSize
	);
}