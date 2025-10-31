#pragma once
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct GameState;
class ResourceStore;


namespace Construct
{
	entt::entity MoveRegionEntity(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
		const Component::Transform&& transform,
		Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);


	entt::entity MoveRegionEntity(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
		Direction region, Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);
}