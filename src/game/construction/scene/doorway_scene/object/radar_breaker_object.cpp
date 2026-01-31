#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include  "game/component/shared/mechanical/breaker_component.hpp"
#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"
#include "game/state/scene.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"


void Object::RadarBreaker::Create(const SceneContext& context, const entt::entity radar)
{
	LeverHandle::Create(context, radar);
	LeverBase::Create(context);
}


entt::entity Object::RadarBreaker::LeverBase::Create(const SceneContext& context)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(60.0f, 90.0f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);
	// TODO: Add visuals
	context.registry.emplace<Component::Rectangle>(entity, Nc::RGBa(GRAY));

	return entity;
}


entt::entity Object::RadarBreaker::LeverHandle::Create(const SceneContext& context, const entt::entity radar)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);
	constexpr auto transform = Component::Transform(Doorway, POSITION, SIZE, SIZE * 0.5f);

	const CircuitBreaker::Data breakerData = CircuitBreaker::Create(context.registry, transform, radar);
	breakerData.breaker.onRestart.connect<&System::Radar::Stability::Restart>();

	// TODO: Add visuals
	context.registry.emplace<Component::Rectangle>(breakerData.entity, Nc::RGBa(RAYWHITE));

	return breakerData.entity;
}
