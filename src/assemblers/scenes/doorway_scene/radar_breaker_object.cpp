#include "assemblers/entities/interactive/circuit_breaker_object.hpp"
#include "assemblers/scenes/doorway_scene/radar_breaker_object.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include "raylib.h"
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


static const entt::entity RadarLeverHandleEntity(entt::registry& registry, ResourceStore& resourceStore, const entt::entity radar)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);

	Component::Transform transform = Component::Transform(Doorway, POSITION, SIZE, SIZE * 0.5f);

	Assembled::CircuitBreakerData breakerData = Construct::CircuitBreakerObject(registry, std::move(transform), radar);

	registry.emplace<Component::Rectangle>(breakerData.entity, RAYWHITE);
	breakerData.breaker.onRestart.connect<&RadarStabilitySystem::Restart>();

	return breakerData.entity;
}


void Construct::RadarBreakerObject(entt::registry& registry, ResourceStore& resourceStore, const entt::entity radar)
{
	RadarLeverHandleEntity(registry, resourceStore, radar);
	RadarLeverBaseEntity(registry, resourceStore);
}