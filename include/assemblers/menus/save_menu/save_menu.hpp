#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;
struct SaveContext;


namespace SaveMenu
{
	void Build(
		entt::registry& registry, ResourceStore& resourceStore, SaveContext& saveContext, GameState& gameState
	);
}