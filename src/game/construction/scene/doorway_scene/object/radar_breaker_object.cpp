#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"
#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"

// TODO: Add visuals


static const entt::entity RadarLeverBaseEntity(entt::registry& registry, Nc::ResourceStore& resourceStore)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(60.0f, 90.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);
	registry.emplace<Component::Rectangle>(entity, GRAY);

	return entity;
}


static const entt::entity RadarLeverHandleEntity(entt::registry& registry, Nc::ResourceStore& resourceStore, const entt::entity radar)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);

	Component::Transform transform = Component::Transform(Doorway, POSITION, SIZE, SIZE * 0.5f);

	Assembled::CircuitBreakerData breakerData = Construct::CircuitBreakerObject(registry, std::move(transform), radar);

	registry.emplace<Component::Rectangle>(breakerData.entity, RAYWHITE);
	breakerData.breaker.onRestart.connect<&RadarStabilitySystem::Restart>();

	return breakerData.entity;
}


void Construct::RadarBreakerObject(entt::registry& registry, Nc::ResourceStore& resourceStore, const entt::entity radar)
{
	RadarLeverHandleEntity(registry, resourceStore, radar);
	RadarLeverBaseEntity(registry, resourceStore);
}