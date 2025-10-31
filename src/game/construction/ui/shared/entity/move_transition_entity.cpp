#include "assemblers/entities/move_transition_entity.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/core/tween_component.hpp"
#include "components/scene/move_region_tag.hpp"
#include "components/ui/move_transition_tag.hpp"
#include "core/context/render_context.hpp"
#include "core/scene.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color_palette.hpp"
#include "utility/tween.hpp"
#include "utility/vector2.hpp"
#include <functional>


void MoveTransition::StartMoveScene(
	entt::registry& registry, GameState& gameState, Scene nextScene, float moveTime
)
{
	entt::entity entity = registry.view<Tag::MoveTransition>().front();
	Component::TweenCollection& collection = registry.get<Component::TweenCollection>(entity);
	collection.tweens.at(MoveTransition::Tweens::TransitionDown).delayComplete = moveTime;
	
	auto view = registry.view<Tag::MoveRegion, Component::ButtonAction>();
	for (auto [entity, button] : view.each()) button.isActive = false;

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


void MoveTransition::InstantTransition(
	entt::registry& registry, GameState& gameState, Scene nextScene
)
{
	entt::entity entity = registry.view<Tag::MoveTransition>().front();
	Component::TweenCollection& collection = registry.get<Component::TweenCollection>(entity);

	gameState.currentScene = nextScene;
	gameState.movingToScene = NullScene;

	Tween::Play(collection.tweens.at(MoveTransition::Tweens::TransitionUp));
}


const entt::entity Construct::MoveTransitionEntity(
	entt::registry& registry, 
	RenderContext& renderContext,
	GameState& gameState
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);
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
	tweenUp.onComplete = [&registry]()
	{
		auto view = registry.view<Tag::MoveRegion, Component::ButtonAction>();
		for (auto [entity, button] : view.each()) button.isActive = true;
	};

	return entity;
}
