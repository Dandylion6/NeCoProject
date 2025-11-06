#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
struct GameState;
namespace Nc { class ResourceStore; };


namespace Construct
{
	entt::entity MoveRegionEntity(
		entt::registry& registry, GameState& gameState, Nc::ResourceStore& resourceStore,
		const Component::Transform&& transform,
		Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);


	entt::entity MoveRegionEntity(
		entt::registry& registry, GameState& gameState, Nc::ResourceStore& resourceStore,
		Direction region, Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);
}