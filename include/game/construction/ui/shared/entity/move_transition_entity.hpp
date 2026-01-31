#pragma once
#include "entt/entity/fwd.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"
namespace Nc { struct RenderContext; };


namespace Entity
{

class MoveTransition final
{
public:
	static entt::entity Create(const SceneContext& sceneContext, const Nc::RenderContext& renderContext) noexcept;
	static void StartMoveScene(const SceneContext& context, Scene nextScene, float moveTime) noexcept;
	static void InstantTransition(const SceneContext& context, Scene nextScene) noexcept;

private:
	enum Tweens
	{
		TransitionDown = 0u,
		TransitionUp = 1u,
	};

};

}