#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/shared/move_region_tag.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
#include <functional>
#include <utility>


entt::entity Construct::MoveRegionEntity(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
	const Component::Transform&& transform,
	Scene currentScene, Scene nextScene, float moveTime
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MoveRegion>(entity);
	registry.emplace<Component::Transform>(entity, transform);

	std::function<void()> onClick = [&registry, &gameState, &resourceStore, nextScene, moveTime]()
		{
			if (gameState.movingToScene != NullScene) return;
			gameState.movingToScene = nextScene;

			MoveTransition::StartMoveScene(registry, gameState, nextScene, moveTime);

			Sound& transitionSound = resourceStore.GetSound("assets/audio/effects/scene_transition.wav");
			PlaySound(transitionSound);

		};

	registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
	return entity;
}


entt::entity Construct::MoveRegionEntity(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
	Direction region, Scene currentScene, Scene nextScene, float moveTime
)
{
	Component::Transform transform = Component::Transform(currentScene);
	Nc::Vector2f displaySize = Nc::RENDER_RESOLUTION;
	constexpr float WIDTH_MULTIPLIER = 0.1f, HEIGHT_MULTIPLIER = 0.2f;

	switch (region)
	{
	case Up:
	{
		Nc::Vector2f regionSize = Nc::Vector2f::Up(displaySize.y * HEIGHT_MULTIPLIER);
		regionSize.x = displaySize.x * (1.0f - WIDTH_MULTIPLIER * 2.0f);

		transform.size = regionSize;
		transform.position.x = displaySize.x * 0.5f;
		transform.offset.x = regionSize.x * 0.5f;
		break;
	}
	case Down:
	{
		Nc::Vector2f regionSize = Nc::Vector2f::Up(displaySize.y * HEIGHT_MULTIPLIER);
		regionSize.x = displaySize.x * (1.0f - WIDTH_MULTIPLIER * 2.0f);

		transform.size = regionSize;
		transform.position = Nc::Vector2f(displaySize.x * 0.5f, displaySize.y);
		transform.offset = Nc::Vector2f(regionSize.x * 0.5f, regionSize.y);
		break;
	}
	case Left:
	{
		Nc::Vector2f regionSize = Nc::Vector2f::Up(displaySize.y);
		regionSize.x = displaySize.x * WIDTH_MULTIPLIER;
		transform.size = regionSize;
		break;
	}
	case Right:
	{
		Nc::Vector2f regionSize = Nc::Vector2f::Up(displaySize.y);
		regionSize.x = displaySize.x * WIDTH_MULTIPLIER;

		transform.size = regionSize;
		transform.position.x = displaySize.x;
		transform.offset.x = regionSize.x;
		break;
	}
	}
	return MoveRegionEntity(
		registry, gameState, resourceStore, std::move(transform), 
		currentScene, nextScene, moveTime
	);
}
