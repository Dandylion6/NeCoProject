#include "game/construction/ui/shared/entity/move_transition_entity.hpp"

#include <functional>

#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/shared/move_region_tag.hpp"
#include "game/tag/ui/move_transition_tag.hpp"
#include "game/utility/color_palette.hpp"


entt::entity Entity::MoveTransition::Create(const SceneContext& sceneContext, const Nc::RenderContext& renderContext) noexcept
{
    const entt::entity entity = sceneContext.registry.create();

	auto size = Nc::Vector2f(renderContext.windowSize);
	auto& transform = sceneContext.registry.emplace<Component::UI::Transform>(entity, Nc::Vector2f::Zero(), Nc::Vector2f::Up(), size);
	sceneContext.registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

	//Construct transition tweening
	sceneContext.registry.emplace<Tag::MoveTransition>(entity);
	auto& collection = sceneContext.registry.emplace<Component::TweenCollection>(entity);

	//Transition down
	Nc::Tween& tweenDown = collection.tweens.at(TransitionDown);
	Nc::Tween::Build(tweenDown, &transform.origin.y, 1.0f, 0.0f, 0.4, SineInOut, 0.2f);

	//Transition up
	Nc::Tween& tweenUp = collection.tweens.at(TransitionUp);
	Nc::Tween::Build(tweenUp, &transform.origin.y, 0.0f, 1.0f, 0.6f, CubicOut);
	tweenUp.onComplete = [&registry = sceneContext.registry]()
	{
		const entt::entity regionEntity = entt::get_single<Tag::MoveRegion>(registry);
		auto& button = registry.get<Component::Action::Click>(regionEntity);
		button.isActive = true;
	};

	return entity;
}


void Entity::MoveTransition::StartMoveScene(const SceneContext& context, Scene nextScene, const float moveTime) noexcept
{
	const entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);

	auto& collection = context.registry.get<Component::TweenCollection>(entity);
	collection.tweens.at(TransitionDown).delayComplete = moveTime;

	const std::function switchScene = [&collection, &gameState = context.game, nextScene]()
	{
		gameState.currentScene = nextScene;
		gameState.movingToScene = NullScene;

		Nc::Tween& tweenUp = collection.tweens.at(TransitionUp);
		Nc::Tween::Play(tweenUp);
	};

	Nc::Tween& tweenDown = collection.tweens.at(TransitionDown);
	tweenDown.onComplete = switchScene;
	Nc::Tween::Play(tweenDown);
}


void Entity::MoveTransition::InstantTransition(const SceneContext& context, const Scene nextScene) noexcept
{
	const entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);
	
	context.game.currentScene = nextScene;
	context.game.movingToScene = NullScene;

	auto& collection = context.registry.get<Component::TweenCollection>(entity);
	Nc::Tween& tween = collection.tweens.at(TransitionUp);
	Nc::Tween::Play(tween);
}