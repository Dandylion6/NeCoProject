#include "game/construction/shared/entity/scene/move_region_entity.hpp"

#include <functional>
#include <utility>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/shared/move_region_tag.hpp"


entt::entity Entity::MoveRegion::Create(
	const SceneContext& context,
	const Component::Transform& transform,
	Scene nextScene,
	float moveTime
) noexcept
{
	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::MoveRegion>(entity);
	context.registry.emplace<Component::Transform>(entity, transform);

	std::function onClick = [context, nextScene, moveTime]()
	{
		constexpr char TRANSITION_SOUND_PATH[] = "assets/audio/effects/scene_transition.wav";

		if (context.game.movingToScene != NullScene) return;
		context.game.movingToScene = nextScene;

		MoveTransition::StartMoveScene(context, nextScene, moveTime);

		const Sound& transitionSound = context.store.GetSound(TRANSITION_SOUND_PATH);
		PlaySound(transitionSound);
	};

	context.registry.emplace<Component::Action::Click>(entity, std::move(onClick));
	return entity;
}


entt::entity Entity::MoveRegion::Create(
	const SceneContext& context,
	const Direction region,
	const Scene currentScene,
	const Scene nextScene,
	const float moveTime
) noexcept
{
	constexpr Nc::Vector2f displaySize = Nc::Vector2f(Nc::RENDER_RESOLUTION);
	constexpr float WIDTH_MULTIPLIER = 0.1f, HEIGHT_MULTIPLIER = 0.2f;

	auto transform = Component::Transform(currentScene);

	switch (region)
	{
	case Up:
	{
		auto regionSize = Nc::Vector2f::Up(displaySize.y * HEIGHT_MULTIPLIER);
		regionSize.x = displaySize.x * (1.0f - WIDTH_MULTIPLIER * 2.0f);

		transform.size = regionSize;
		transform.position.x = displaySize.x * 0.5f;
		transform.offset.x = regionSize.x * 0.5f;
		break;
	}
	case Down:
	{
		auto regionSize = Nc::Vector2f::Up(displaySize.y * HEIGHT_MULTIPLIER);
		regionSize.x = displaySize.x * (1.0f - WIDTH_MULTIPLIER * 2.0f);

		transform.size = regionSize;
		transform.position = Nc::Vector2f(displaySize.x * 0.5f, displaySize.y);
		transform.offset = Nc::Vector2f(regionSize.x * 0.5f, regionSize.y);
		break;
	}
	case Left:
	{
		auto regionSize = Nc::Vector2f::Up(displaySize.y);
		regionSize.x = displaySize.x * WIDTH_MULTIPLIER;
		transform.size = regionSize;
		break;
	}
	case Right:
	{
		auto regionSize = Nc::Vector2f::Up(displaySize.y);
		regionSize.x = displaySize.x * WIDTH_MULTIPLIER;

		transform.size = regionSize;
		transform.position.x = displaySize.x;
		transform.offset.x = regionSize.x;
		break;
	}
	}

	return Create(context, transform, nextScene, moveTime);
}
