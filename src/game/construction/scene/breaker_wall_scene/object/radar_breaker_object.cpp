#include "../../../../../../include/game/construction/scene/breaker_wall_scene/object/radar_breaker_object.hpp"

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
	const entt::entity base = LeverBase::Create(context);
	const CircuitBreaker::Data data = LeverHandle::Create(context, radar, base);
    Indicator::Create(context, data.indicator);
}


entt::entity Object::RadarBreaker::LeverBase::Create(const SceneContext& context)
{
    constexpr char LEVER_ALBEDO_PATH[] = "assets/environment/objects/switches/comms_breaker/switch/comms_breaker_switch_albedo.png";
    constexpr char LEVER_NORMAL_PATH[] = "assets/environment/objects/switches/comms_breaker/switch/comms_breaker_switch_normal.png";
    constexpr char LEVER_AO_PATH[] = "assets/environment/objects/switches/comms_breaker/switch/comms_breaker_switch_ao.png";

	constexpr Nc::Vector2f POSITION = Nc::Vector2f(409.0f, 220.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(80.0f, 213.0f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, BreakerWall, POSITION, SIZE, SIZE * 0.5f);

    const Texture2D& albedoTexture = context.store.GetTexture(LEVER_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(LEVER_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(LEVER_AO_PATH);

    context.registry.emplace<Component::Sprite>(entity, albedoTexture, normalTexture, aoTexture, 9u);

    return entity;
}


Object::CircuitBreaker::Data Object::RadarBreaker::LeverHandle::Create(
    const SceneContext& context,
    const entt::entity radar,
    const entt::entity base
)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(409.0f, 220.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(70.0f, 50.0f);
	constexpr auto TRANSFORM = Component::Transform(BreakerWall, POSITION, SIZE, SIZE * 0.5f);

	const CircuitBreaker::Data breakerData = CircuitBreaker::Create(context.registry, TRANSFORM, 80.0f, radar, base);
	breakerData.breaker.onRestart.connect<&System::Radar::Stability::Restart>();

	return breakerData;
}


entt::entity Object::RadarBreaker::Indicator::Create(const SceneContext& context, const entt::entity indicator)
{
    constexpr char ALBEDO_PATH[] = "assets/environment/objects/switches/comms_breaker/indicator/comms_breaker_indicator_albedo.png";
    constexpr char NORMAL_PATH[] = "assets/environment/objects/switches/comms_breaker/indicator/comms_breaker_indicator_normal.png";
    constexpr char AO_PATH[] = "assets/environment/objects/switches/comms_breaker/indicator/comms_breaker_indicator_ao.png";

    constexpr auto POSITION = Nc::Vector2f(460.0f, 173.0f);
    constexpr auto SIZE = Nc::Vector2f(13.0f, 13.0f);

    auto& transform = context.registry.get<Component::Transform>(indicator);
    transform.position = POSITION;
    transform.boundScene = BreakerWall;
    transform.size = SIZE;
    transform.offset = SIZE * 0.5f;

    const Texture2D& albedoTexture = context.store.GetTexture(ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(AO_PATH);

    auto& sprite = context.registry.get<Component::Sprite>(indicator);
    sprite.albedo = albedoTexture;
    sprite.normals = normalTexture;
    sprite.ambientOcclusion = aoTexture;

    return indicator;
}
