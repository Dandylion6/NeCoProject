#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace RestartMenu
{
	void Build(entt::registry& registry, GameState& gameState, ResourceStore& resourceStore);

	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
}