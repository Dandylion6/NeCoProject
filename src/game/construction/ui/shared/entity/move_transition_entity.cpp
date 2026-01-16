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
#include "core/runtime/entity_helpers.hpp"
#include <functional>


entt::entity Entity::MoveTransition::Create(SceneContext sceneContext, Nc::RenderContext& renderContext) noexcept
{
    entt::entity entity = sceneContext.registry.create();

	Nc::Vector2f size = Nc::Vector2f(renderContext.windowSize);
	Component::UI::Transform& transform = sceneContext.registry.emplace<Component::UI::Transform>(
		entity,
		Nc::Vector2f::Zero(),
		Nc::Vector2f::Up(),
		size
	);
	sceneContext.registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

	//Construct transition tweening
	sceneContext.registry.emplace<Tag::MoveTransition>(entity);
	Component::TweenCollection& collection = sceneContext.registry.emplace<Component::TweenCollection>(entity);

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
	tweenUp.onComplete = [&registry = sceneContext.registry]()
	{
		auto view = registry.view<Tag::MoveRegion, Component::Action::Click>();
		for (auto [entity, button] : view.each()) button.isActive = true;
	};

	return entity;
}


void Entity::MoveTransition::StartMoveScene(SceneContext context, Scene nextScene, float moveTime) noexcept
{
	entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);

	Component::TweenCollection& collection = context.registry.get<Component::TweenCollection>(entity);
	collection.tweens.at(MoveTransition::Tweens::TransitionDown).delayComplete = moveTime;

	std::function<void()> switchScene = [&collection, &gameState = context.game, nextScene]()
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


void Entity::MoveTransition::InstantTransition(SceneContext context, Scene nextScene) noexcept
{
	entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);
	
	context.game.currentScene = nextScene;
	context.game.movingToScene = NullScene;
	
	Component::TweenCollection& collection = context.registry.get<Component::TweenCollection>(entity);
	Tween& tween = collection.tweens.at(MoveTransition::Tweens::TransitionUp);
	Tween::Play(tween);
}