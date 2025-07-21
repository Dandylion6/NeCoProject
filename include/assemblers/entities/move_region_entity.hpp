#pragma once
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
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