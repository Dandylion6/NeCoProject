#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
class ResourceStore;


namespace MainMenu
{
	void Build(
		Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
	);

	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
}