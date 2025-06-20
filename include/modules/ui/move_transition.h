#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
struct MoveSceneEvent;
struct RenderContext;


namespace Construct
{
	entt::entity MoveTransitionEntity(
		entt::registry& registry, 
		RenderContext& renderContext,
		GameState& gameState
	);
}


namespace MoveTransition
{
	enum Tweens
	{
		TransitionDown = 0u,
		TransitionUp = 1u,
	};

	void Build(
		entt::registry& registry,
		RenderContext& renderContext,
		GameState& gameState
	);

	void StartMoveScene(
		entt::registry& registry, Scene& currentScene, Scene nextScene, float moveTime
	);
}