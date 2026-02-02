#include "game/construction/ui/shared/entity/move_transition_entity.hpp"

#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
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
	Nc::Tween& tweenDown = collection.tweens.at(Tag::MoveTransition::TransitionDown);
	Nc::Tween::Build(tweenDown, &transform.origin.y, 1.0f, 0.0f, 0.4, SineInOut, 0.2f);

	//Transition up
	Nc::Tween& tweenUp = collection.tweens.at(Tag::MoveTransition::TransitionUp);
	Nc::Tween::Build(tweenUp, &transform.origin.y, 0.0f, 1.0f, 0.6f, CubicOut);
	return entity;
}