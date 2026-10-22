#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
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
    constexpr char SCENE_ALBEDO_PATH[] = "assets/environment/objects/switches/comms_breaker_switch/comms_breaker_switch_albedo.png";
    constexpr char SCENE_NORMAL_PATH[] = "assets/environment/objects/switches/comms_breaker_switch/comms_breaker_switch_normal.png";
    constexpr char SCENE_AO_PATH[] = "assets/environment/objects/switches/comms_breaker_switch/comms_breaker_switch_ao.png";
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(400.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(100.0f, 100.0f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, Doorway, POSITION, SIZE, SIZE * 0.5f);

    const Texture2D& sceneAlbedoTexture = context.store.GetTexture(SCENE_ALBEDO_PATH);
    const Texture2D& sceneNormalTexture = context.store.GetTexture(SCENE_NORMAL_PATH);
    const Texture2D& sceneAoTexture = context.store.GetTexture(SCENE_AO_PATH);

    context.registry.emplace<Component::Sprite>(entity, sceneAlbedoTexture, sceneNormalTexture, sceneAoTexture, 5u);

    return entity;
}


entt::entity Object::RadarBreaker::LeverHandle::Create(const SceneContext& context, const entt::entity radar)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(200.0f, 100.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(55.0f, 20.0f);
	constexpr auto TRANSFORM = Component::Transform(Doorway, POSITION, SIZE, SIZE * 0.5f);

	const CircuitBreaker::Data breakerData = CircuitBreaker::Create(context.registry, TRANSFORM, radar);
	breakerData.breaker.onRestart.connect<&System::Radar::Stability::Restart>();

	// TODO: Add visuals
	context.registry.emplace<Component::Rectangle>(breakerData.entity, Nc::RGBa(RAYWHITE));

	return breakerData.entity;
}
