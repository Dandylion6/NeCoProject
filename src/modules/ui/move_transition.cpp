#include "components/core/rendering/rectangle_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/tags/move_transition_tag.h"
#include "core/ecs_context.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "entt/signal/dispatcher.hpp"
#include "entt/signal/sigh.hpp"
#include "modules/ui/move_transition.h"
#include "utility/entity/move_entities.h"
#include "utility/tween.h"
#include "utility/vector2.h"
#include <functional>


void MoveTransition::Build(
	EcsContext& ecsContext, 
	RenderContext& renderContext,
	GameState& gameState
)
{
	Construct::MoveTransitionEntity(
		ecsContext, renderContext, gameState
	);
}


void MoveTransition::StartMoveScene(MoveSceneEvent& event)
{
	entt::registry& registry = *event.registry;
	entt::entity entity = registry.view<Tag::MoveTransition>().front();

	Component::TweenCollection& collection = registry.get<Component::TweenCollection>(entity);
	collection.tweens.at(TransitionDown).delayComplete = event.moveTime;
	
	std::function<void()> switchScene = std::function<void()>(
		[&collection, currentScene = event.currentScene, nextScene = event.nextScene]()
		{
			*currentScene = nextScene;
			Tween::Play(collection.tweens.at(TransitionUp));
		}
	);

	Tween& tweenDown = collection.tweens.at(TransitionDown);
	tweenDown.onComplete = switchScene;
	Tween::Play(tweenDown);
}


entt::entity Construct::MoveTransitionEntity(
	EcsContext& ecsContext, 
	RenderContext& renderContext,
	GameState& gameState
)
{
	entt::entity entity = ecsContext.registry.create();
	ecsContext.registry.emplace<Component::Rectangle>(entity, RenderContext::CLEAR_COLOR);
	Component::UiTransform& transform = ecsContext.registry.emplace<Component::UiTransform>(
		entity,
		Nc::Vector2f::Zero(),
		Nc::Vector2f::Up(),
		renderContext.windowSize
	);


	//Construct transition tweening
	ecsContext.registry.emplace<Tag::MoveTransition>(entity);
	Component::TweenCollection& collection = ecsContext.registry.emplace<Component::TweenCollection>(entity);

	//Transition down
	Tween& tweenDown = collection.tweens.at(MoveTransition::TransitionDown);
	tweenDown.value = &transform.origin.y;
	tweenDown.start = 1.0f;
	tweenDown.end = 0.0f;
	tweenDown.duration = 0.4f;
	tweenDown.easing = SineInOut;
	tweenDown.delayComplete = 0.2f;

	//Transition up
	Tween& tweenUp = collection.tweens.at(MoveTransition::TransitionUp);
	tweenUp.value = &transform.origin.y;
	tweenUp.start = 0.0f;
	tweenUp.end = 1.0f;
	tweenUp.duration = 0.6f;
	tweenUp.easing = CubicOut;
	tweenUp.onComplete = [&gameState]()
	{
		gameState.movingToScene = NullScene;
	};

	ecsContext.dispatcher.sink<MoveSceneEvent>().connect<&MoveTransition::StartMoveScene>();
	return entity;
}
