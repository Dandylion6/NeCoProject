#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
class ResourceStore;


namespace RestartMenu
{
	void Build(entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, Nc::Vector2f windowSize);

	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
}