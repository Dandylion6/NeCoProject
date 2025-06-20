#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace DoorwayScene
{
	void Build(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}