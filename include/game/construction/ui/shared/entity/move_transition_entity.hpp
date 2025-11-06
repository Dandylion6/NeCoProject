#pragma once
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { struct RenderContext; };


namespace Construct
{
	const entt::entity MoveTransitionEntity(
		entt::registry& registry, 
		Nc::RenderContext& renderContext,
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
		entt::registry& registry, 
		GameState& gameState, 
		Scene nextScene, 
		float moveTime
	);

	void InstantTransition(
		entt::registry& registry,
		GameState& gameState,
		Scene nextScene
	);
}