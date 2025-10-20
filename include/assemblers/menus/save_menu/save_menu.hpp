#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
class ResourceStore;
struct SaveContext;


namespace SaveMenu
{
	void Build(
		entt::registry& registry, ResourceStore& resourceStore, SaveContext& saveContext, GameState& gameState, Nc::Vector2f windowSize
	);

	void Open(entt::registry& registry, SaveContext& saveContext);

	void Close(entt::registry& registry);
}