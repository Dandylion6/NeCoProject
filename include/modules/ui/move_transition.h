#pragma once
#include "entt/entity/fwd.hpp"
struct EcsContext;
struct GameState;
struct MoveSceneEvent;
struct RenderContext;


namespace Construct
{
	entt::entity MoveTransitionEntity(
		EcsContext& ecsContext, 
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
		EcsContext& ecsContext,
		RenderContext& renderContext,
		GameState& gameState
	);

	void StartMoveScene(MoveSceneEvent& event);
}