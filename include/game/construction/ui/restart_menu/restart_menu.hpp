#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };
struct SaveContext;


namespace RestartMenu
{
	void Build(
		Game& game, 
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore, 
		Nc::Vector2f windowSize
	);

	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
}