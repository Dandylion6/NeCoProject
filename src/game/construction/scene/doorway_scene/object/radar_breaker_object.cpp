#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"
#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"
#include "game/state/scene.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"
#include "raylib.h"
#include <utility>

// TODO: Add visuals


const entt::entity Object::RadarBreaker::LeverBase::Create(
	entt::registry& registry, Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(60.0f, 90.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);
	registry.emplace<Component::Rectangle>(entity, GRAY);

	return entity;
}


const entt::entity Object::RadarBreaker::LeverHandle::Create(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	const entt::entity radar
) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);

	Component::Transform transform = Component::Transform(Doorway, POSITION, SIZE, SIZE * 0.5f);
	Object::CircuitBreaker::Data breakerData = Object::CircuitBreaker::Create(registry, std::move(transform), radar);
	breakerData.breaker.onRestart.connect<&RadarStabilitySystem::Restart>();

	registry.emplace<Component::Rectangle>(breakerData.entity, RAYWHITE);

	return breakerData.entity;
}


void Object::RadarBreaker::Create(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	const entt::entity radar
) noexcept
{
	Object::RadarBreaker::LeverHandle::Create(registry, resourceStore, radar);
	Object::RadarBreaker::LeverBase::Create(registry, resourceStore);
}
