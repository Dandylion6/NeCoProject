#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace SaveMenu
{
	void Build(entt::registry& registry, ResourceStore& resourceStore, GameState& gameState);
}