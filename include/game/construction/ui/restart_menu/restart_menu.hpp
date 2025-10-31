#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
class Game;
struct GameState;
class ResourceStore;
struct SaveContext;


namespace RestartMenu
{
	void Build(
		Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, Nc::Vector2f windowSize
	);

	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
}