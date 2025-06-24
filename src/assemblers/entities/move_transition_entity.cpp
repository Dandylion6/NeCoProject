#include "assemblers/entities/move_transition_entity.h"
#include "components/core/rendering/rectangle_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/ui/move_transition_tag.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/tween.h"
#include "utility/vector2.h"
#include <functional>


void MoveTransition::StartMoveScene(
	entt::registry& registry, GameState& gameState, Scene nextScene, float moveTime
)
{
	entt::entity entity = registry.view<Tag::MoveTransition>().front();
	Component::TweenCollection& collection = registry.get<Component::TweenCollection>(entity);
	collection.tweens.at(MoveTransition::Tweens::TransitionDown).delayComplete = moveTime;
	
	std::function<void()> switchScene = [&collection, &gameState, nextScene]()
	{
		gameState.currentScene = nextScene;
		gameState.movingToScene = NullScene;

		Tween::Play(collection.tweens.at(MoveTransition::Tweens::TransitionUp));
	};

	Tween& tweenDown = collection.tweens.at(MoveTransition::Tweens::TransitionDown);
	tweenDown.onComplete = switchScene;
	Tween::Play(tweenDown);
}


entt::entity Construct::MoveTransitionEntity(
	entt::registry& registry, 
	RenderContext& renderContext,
	GameState& gameState
)
{
	entt::entity entity = registry.create();
	registry.emplace<Component::Rectangle>(entity, RenderContext::CLEAR_COLOR);
	Component::UiTransform& transform = registry.emplace<Component::UiTransform>(
		entity,
		Nc::Vector2f::Zero(),
		Nc::Vector2f::Up(),
		renderContext.windowSize
	);


	//Construct transition tweening
	registry.emplace<Tag::MoveTransition>(entity);
	Component::TweenCollection& collection = registry.emplace<Component::TweenCollection>(entity);

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

	return entity;
}
