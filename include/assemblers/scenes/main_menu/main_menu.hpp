#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace MainMenu
{
	void Build(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);

	void Open(entt::registry& registry, GameState& gameState);
}