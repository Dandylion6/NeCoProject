#pragma once
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"
struct GameState;
namespace Nc { struct RenderContext; };


namespace Entity
{

class MoveTransition final
{
public:
	static entt::entity Create(
		entt::registry& registry, 
		Nc::RenderContext& renderContext,
		GameState& gameState
	) noexcept;


	static void StartMoveScene(
		entt::registry& registry, 
		GameState& gameState, 
		Scene nextScene, 
		float moveTime
	) noexcept;


	static void InstantTransition(
		entt::registry& registry,
		GameState& gameState,
		Scene nextScene
	) noexcept;

private:
	enum Tweens
	{
		TransitionDown = 0u,
		TransitionUp = 1u,
	};

};

}