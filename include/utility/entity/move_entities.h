#pragma once
#include "components/core/transform_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"
struct EcsContext;
struct GameState;
struct RenderContext;
class ResourceStore;


struct MoveSceneEvent
{
	Scene* currentScene = nullptr;
	entt::registry* registry = nullptr;
	float moveTime = 0.2f;
	Scene nextScene = CommsDesk;
};


namespace Construct
{
	entt::entity MoveRegionEntity(
		EcsContext& ecsContext, GameState& gameState, ResourceStore& resourceStore,
		const Component::Transform& transform,
		Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);


	entt::entity MoveRegionEntity(
		EcsContext& ecsContext, GameState& gameState, ResourceStore& resourceStore,
		Direction region, Scene currentScene, Scene nextScene, float moveTime = 0.2f
	);
}