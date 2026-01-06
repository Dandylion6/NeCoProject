#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/tag/shared/move_region_tag.hpp"
#include "game/tag/ui/move_transition_tag.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/scene.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/utility/color_palette.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include <functional>


entt::entity Entity::MoveTransition::Create(
	entt::registry& registry, 
	Nc::RenderContext& renderContext, 
	GameState& gameState
) noexcept
{
    entt::entity entity = registry.create();

	Nc::Vector2f size = Nc::Vector2f(renderContext.windowSize);
	Component::UI::Transform& transform = registry.emplace<Component::UI::Transform>(
		entity,
		Nc::Vector2f::Zero(),
		Nc::Vector2f::Up(),
		size
	);
	registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

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
		auto view = registry.view<Tag::MoveRegion, Component::Action::Click>();
		for (auto [entity, button] : view.each()) button.isActive = true;
	};

	return entity;
}


void Entity::MoveTransition::StartMoveScene(
	entt::registry& registry, 
	GameState& gameState, 
	Scene nextScene, 
	float moveTime
) noexcept
{
	auto view = registry.view<const Tag::MoveTransition, Component::TweenCollection>();
	for (auto [entity, collection] : view.each())
	{
		collection.tweens.at(MoveTransition::Tweens::TransitionDown).delayComplete = moveTime;
	
		std::function<void()> switchScene = [&collection, &gameState, nextScene]()
		{
			gameState.currentScene = nextScene;
			gameState.movingToScene = NullScene;

			Tween& tweenUp = collection.tweens.at(MoveTransition::Tweens::TransitionUp);
			Tween::Play(tweenUp);
		};

		Tween& tweenDown = collection.tweens.at(MoveTransition::Tweens::TransitionDown);
		tweenDown.onComplete = switchScene;
		Tween::Play(tweenDown);
	}
}


void Entity::MoveTransition::InstantTransition(
	entt::registry& registry, 
	GameState& gameState, 
	Scene nextScene
) noexcept
{
	auto view = registry.view<const Tag::MoveTransition, Component::TweenCollection>();
	for (auto [entity, collection] : view.each())
	{
		gameState.currentScene = nextScene;
		gameState.movingToScene = NullScene;

		Tween& tween = collection.tweens.at(MoveTransition::Tweens::TransitionUp);
		Tween::Play(tween);
	}
}