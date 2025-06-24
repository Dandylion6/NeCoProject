#pragma once
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
struct GameState;
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

	void StartMoveScene(
		entt::registry& registry, GameState& gameState, 
		Scene nextScene, float moveTime
	);
}