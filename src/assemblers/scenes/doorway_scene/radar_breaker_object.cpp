#include "assemblers/scenes/doorway_scene/radar_breaker_object.hpp"
#include "components/core/drag_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/breaker/radar_restart_system.hpp"
#include "utility/vector2.hpp"

// TODO: Add visuals


static const entt::entity RadarLeverBaseEntity(entt::registry& registry, ResourceStore& resourceStore)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(60.0f, 90.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);
	registry.emplace<Component::Rectangle>(entity, GRAY);

	return entity;
}


static const entt::entity RadarLeverHandleEntity(entt::registry& registry, ResourceStore& resourceStore)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 70.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);
	registry.emplace<Component::Rectangle>(entity, RAYWHITE);

	Component::DragAction& drag = registry.emplace<Component::DragAction>(entity);
	drag.onDragging.connect<&RadarRestartSystem::OnLeverDrag>();
	drag.onReleased.connect<&RadarRestartSystem::OnLeverRelease>();

	return entity;
}


void Construct::RadarBreakerObject(entt::registry& registry, ResourceStore& resourceStore)
{
	RadarLeverHandleEntity(registry, resourceStore);
	RadarLeverBaseEntity(registry, resourceStore);
}