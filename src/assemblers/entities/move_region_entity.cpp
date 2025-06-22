#include "assemblers/entities/move_region_entity.h"
#include "assemblers/entities/move_transition_entity.h"
#include "components/core/button_action_component.h"
#include "components/core/transform_component.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.h"
#include <functional>
#include <utility>


entt::entity Construct::MoveRegionEntity(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
	const Component::Transform& transform,
	Scene currentScene, Scene nextScene, float moveTime
)
{
	const entt::entity entity = registry.create();
	registry.emplace<Component::Transform>(entity, transform);

	std::function<void()> onClick = std::function<void()>(
		[&registry, &gameState, &resourceStore, nextScene, moveTime]()
	{
		if (gameState.movingToScene != NullScene) return;
		gameState.movingToScene = nextScene;

		MoveTransition::StartMoveScene(registry, gameState.currentScene, nextScene, moveTime);

		Sound transitionSound = LoadSound("assets/audio/effects/scene_transition.wav");
		PlaySound(transitionSound);
	}
	);

	registry.emplace<Component::ButtonAction>(entity, std::move(onClick));
	return entity;
}


entt::entity Construct::MoveRegionEntity(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore,
	Direction region, Scene currentScene, Scene nextScene, float moveTime
)
{
	Component::Transform transform = Component::Transform(currentScene);
	Nc::Vector2f displaySize = RenderContext::DISPLAY_SIZE;
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
		registry, gameState, resourceStore, transform, currentScene, nextScene, moveTime
	);
}
